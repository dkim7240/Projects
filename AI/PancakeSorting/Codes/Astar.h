// By Dokyun Kim (dkim11)

#pragma once

#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
#include "PQueue.h"


#define SIZE 5

using namespace std;

/* A* algorithm module to reach the goal state with 
   the minimum total number of pancakes flipped */

class Astar{
	
	PQueue frontier; 
	Node* start;
	Node* goal;
	Node* visited;

	// Method that returns the value of heuristic function
	int heuristic(vector<int> &val);

	// Method to execute actual A* algorithm to get the solution
	Node* searchA(Node* node);

	// Method to expand the selected node from the frontier
	void selectChild(Node * node, int pos);

	// Method to check whether the node has already been visited
	bool visitedCheck(Node* node);

	// Method to print out the solution for the given start state
	int print(Node* current, int cnt);

public:
		Astar(vector<int> &s);
		~Astar();
	void search();
	
};

#endif
