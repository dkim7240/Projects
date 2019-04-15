#ifndef __BEHAVIORTREE_H
#define __BEHAVIORTREE_H
#endif // !__NODE_H

#include "Nodes.h"

class BehaviorTree {
	Priority* root;
	Sequence* lowBattery;
	Selection* roombaClean;
	DoNothing* doNothing;
	Blackboard* blackboard;
public:
	BehaviorTree();
	void initializeTree();
	void getLowBatteryTree();
	void getRoombaCleanTree();
	Sequence* generalCleanTree();
	bool isCleaningCompleted(int mode);

	int getBattery();
	void setMode(bool general, bool spot);
	void setBattery(int battery);

	int runTree() { 
		int res = root->run();
		if (res == 0) {
			cout << "Tree returns success" << endl;
		}
		else if(res == 2) {
			cout << "Tree returns running" << endl;
		}
		else {
			cout << "Running Tree failed!" << endl;
		}

		return res;
	};

};

#pragma once
