#include "BehaviorTree.h"

BehaviorTree::BehaviorTree()
{
	blackboard = new Blackboard();
	blackboard->bLevel = -1;
	blackboard->docked = false;
}

void BehaviorTree::initializeTree()
{

	root = new Priority(blackboard);

	if (root != NULL) {

		getLowBatteryTree();
		getRoombaCleanTree();
		doNothing = new DoNothing(blackboard);
		doNothing->priority = 3;

		root->add_pChildren(lowBattery);
	    root->add_pChildren(roombaClean);
    	root->add_pChildren(doNothing);

		root->addChildren(lowBattery);
		root->addChildren(roombaClean);
 		root->addChildren(doNothing);
	}

	
}

void BehaviorTree::getLowBatteryTree()
{
	lowBattery = new Sequence(blackboard, false);
	lowBattery->priority = 1;

	lowBattery->addChildren(new CheckBattery(blackboard, false));
	lowBattery->addChildren(new FindHome(blackboard));
	lowBattery->addChildren(new GoHome(blackboard));
	lowBattery->addChildren(new Dock(blackboard));
}

void BehaviorTree::getRoombaCleanTree()
{
	roombaClean = new Selection(blackboard);
	roombaClean->priority = 2;
	
	Sequence* spotcleanTree = new Sequence(blackboard, false);
	spotcleanTree->addChildren(new CheckSpot(blackboard));
	spotcleanTree->addChildren(new CleanSpot(blackboard, 20));
	spotcleanTree->addChildren(new DoneSpot(blackboard));

	roombaClean->addChildren(spotcleanTree);
	roombaClean->addChildren(generalCleanTree());

}

Sequence* BehaviorTree::generalCleanTree()
{
	Selection* generalCleanSub = new Selection(blackboard);
	Sequence* dustyspotTree = new Sequence(blackboard, false);
	Sequence* batteryCheckSub = new Sequence(blackboard, true);
	Sequence* doneGeneralSub = new Sequence(blackboard, false);
	Sequence* rootgeneralclean = new Sequence(blackboard, false);

	dustyspotTree->addChildren(new CheckDustySpot(blackboard));
	dustyspotTree->addChildren(new CleanSpot(blackboard, 35));

	generalCleanSub->addChildren(dustyspotTree);
	generalCleanSub->addChildren(new Clean(blackboard));

	batteryCheckSub->addChildren(new CheckBattery(blackboard, true));
	batteryCheckSub->addChildren(generalCleanSub);


	doneGeneralSub->addChildren(batteryCheckSub);
	doneGeneralSub->addChildren(new DoneGeneral(blackboard));

	rootgeneralclean->addChildren(new CheckGeneral(blackboard));
	rootgeneralclean->addChildren(doneGeneralSub);

	return rootgeneralclean;
}

bool BehaviorTree::isCleaningCompleted(int mode)
{
	bool res = false;

	switch (mode)
	{
	case 1:
		if (blackboard->spot == false) res = true;
		break;
	case 2:
		if (blackboard->general == false) res = true;
		break;
	}

	return res;
}

int BehaviorTree::getBattery()
{
	return blackboard->bLevel;
}

void BehaviorTree::setMode(bool general, bool spot)
{
	blackboard->general = general;
	blackboard->spot = spot;

}


void BehaviorTree::setBattery(int battery)
{
	blackboard->bLevel = battery;
}



