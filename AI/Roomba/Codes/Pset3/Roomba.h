#pragma once

#ifndef __ROOMBA_H
#define __ROOMBA_H
#endif // !__ROOMBA_H

#include <map>

using namespace std;

class Roomba {
private:
	int battery;
	map<string, string> blackBoard;

public:
	Roomba();
	void BatteryCheck();

};
