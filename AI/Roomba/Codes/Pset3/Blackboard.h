#ifndef __BLACK_BOARD_H
#define __BLACK_BOARD_H
#endif // !__BLACK_BOARD_H

#include <string>

class Blackboard
{
public:
	int bLevel;
	bool spot, general, dusty_spot;
	bool docked;
	bool findHomeRequested;
	bool resetTimer;
	std::string homePath;
};

#pragma once
