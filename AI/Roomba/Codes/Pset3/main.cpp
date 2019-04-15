#include <iostream>
#include <string>
#include <map>
#include "BehaviorTree.h"

using namespace std;

/* input : 1. BATTERY_LEVEL: an integer number between 0 and 100 
		   2. SPOT: a Boolean value – TRUE if the command was requested, FALSE otherwise 
		   3. GENERAL: a Boolean value – TRUE if the command was requested, FALSE otherwise 
		   4. DUSTY_SPOT: a Boolean value – TRUE if the sensor detected a dusty spot during the 
						  cycle, FALSE otherwise 
		   5. HOME_PATH: The path to the docking station */

int main()
{
	bool cont = true;
	char flag;
	int bLevel;
	int mode;
	BehaviorTree *bt = new BehaviorTree();


	cout << "Enter the Initial Battery Level: " << endl;
	cin >> bLevel;
	
	bt->initializeTree();
	bt->setBattery(bLevel);

	while (cont) {
		cout << "Would you like to give the commands to Roomba?"
			<< " (Y/N) " << endl;
		cin >> flag;
		if (flag == 'N' || flag == 'n') break;
		cout << "Choose the cleaning mode for Roomba: " << endl;
		cout << "1. Spot cleaning " << endl;
		cout << "2. General cleaning " << endl;
		cout << "3. Do nothing (keep charging if docked)" << endl;

		cin >> mode;

		if (mode == 1) {
			bt->setMode(false, true);
		}
		else if (mode == 2) {
			bt->setMode(true, false);
		}
		else if (mode == 3) {
			bt->setMode(false, false);
			bt->runTree();
		}

		if (mode == 1 || mode == 2) {
			while (!(bt->isCleaningCompleted(mode)))
			{
				int res = bt->runTree();
			}
		}
		
	}

	delete bt;

	system("pause");
	return 0;
}