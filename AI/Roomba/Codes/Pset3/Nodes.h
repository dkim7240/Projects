#ifndef __NODES_H
#define __NODES_H
#endif // !__NODE_H

#include "Blackboard.h"
#include <vector>
#include <functional>
#include <queue>
#include <iostream>
#include <time.h>

using namespace std;

class Node {
protected:
	Blackboard* state;
public:
	Node(Blackboard* b){
		state = b;
	}
	int priority;
	virtual int run() = 0;
	
};

class Compare
{
public:
	bool operator() (Node* left, Node* right)
	{
		return left->priority > right->priority;
	}
};

class Composite : public Node {
protected:
	int res;
	vector<Node*>::iterator current;
	vector<Node*> children;
	priority_queue<Node*, vector<Node*>, Compare> pChildren;

public:
	Composite(Blackboard* b) : Node(b) { res = 0; }
	void addChildren(Node* child) { children.push_back(child); }
	void add_pChildren(Node* child) { pChildren.push(child); }
	vector<Node*>::iterator getChildren() { return children.begin(); }
	priority_queue<Node*, vector<Node*>, Compare> getpChildren()
	{
		return pChildren;
	}
};

class Priority : public Composite {
public:
	Priority(Blackboard * b) : Composite(b) {}

	virtual int run() {
		int res;
		while (!pChildren.empty()) pChildren.pop();
		for (vector<Node*>::iterator child = children.begin(); 
			child != children.end(); ++child) {
			pChildren.push(*child);
		}

		while (!pChildren.empty()) {
			res = pChildren.top()->run();
			if (res == 0) {
				return 0;
			}
			else if (res == 2) {
				return 2;
			}

			pChildren.pop();
		}
		return 1;
	}
};

class Selection : public Composite {
	
public:

	Selection(Blackboard* b) : Composite(b)
	{
	}

	virtual int run() {

		if (res == 0) current = children.begin();
		
		vector<Node*>::iterator child;

		for (child = current; child != children.end(); ++child) {
			current = child;
		    res = (*child)->run();
			if (res == 0) {
				current = children.begin();
				return 0;
			}
			else if (res == 2) {
				return 2;
			}
		}
		current = children.begin();
		return 1;
	}

};

class Sequence : public Composite {
	bool untilFail;
public:
	Sequence(Blackboard* b, bool untilFail) : Composite(b) 
	{
		this->untilFail = untilFail;
	}

	virtual int run() {

		if (res == 0) current = children.begin();
		vector<Node*>::iterator child;
		

		for (child = current; child != children.end(); ++child) {
			current = child;
		    res = (*child)->run();
			if (res == 1) {
				current = children.begin();
				if (untilFail == true) return 0;
				return 1;
			}
			else if (res == 2) {
				return 2;
			}
		}

		current = children.begin();
		if (res == 0 && untilFail == true) return 2;

		return 0;
	}
};

class FindHome : public Node {
	int timer;
public:
	FindHome(Blackboard* b) : Node(b)
	{
		srand(time(NULL));
		// timer = rand() % 4 + 1;

	}

	virtual int run() {
		int res;

		if (state->bLevel == 0) {
			cout << "FIND HOME FAILED" << endl;
			return 1;
		}

		if (state->findHomeRequested) {
			timer = rand() % 4 + 1;
			state->findHomeRequested = false;
		}
		
		if (state->docked == true) timer = 0;

		if (timer > 0) {
			cout << "FIND HOME RUNNING" << endl;
			cout << "timer : " << timer << endl;
			res = 2;
		}	
		else {
			cout << "FIND HOME SUCCEEDED" << endl;
			state->homePath = "HOMEPATH";
			res = 0;
			state->findHomeRequested = true;
		}

		if (timer > 0) timer--;
		if (state->bLevel > 0 && state->docked == false) {
			(state->bLevel)--;
		}

		return res;
	}
};

class GoHome : public Node {
	int timer;
public:

	GoHome(Blackboard* b) : Node(b)
	{
	}

	virtual int run() 
	{
		int res;

		if (state->bLevel == 0) {
			cout << "GO HOME FAILED" << endl;
			return 1;
		}

		if (state->findHomeRequested) {
			timer = rand() % 7 + 1;
			state->findHomeRequested = false;
		}
		if (state->docked == true) timer = 0;
		
		if (timer > 0) {
			cout << "GO HOME RUNNING" << endl;
			cout << "PATH: " << state->homePath << endl;
			res = 2;
		}
		else {
			cout << "GO HOME SUCCEEDED" << endl;
			res = 0;
		}

		if (timer > 0) timer--;
		if (state->bLevel > 0 && state->docked == false) {
			(state->bLevel)--;
		}
		return res;
	}
};

class Dock : public Node {
public:
	Dock(Blackboard* b) : Node(b) {}

	virtual int run() {
		cout << "DOCK SUCCEEDED" << endl;
		state->docked = true;
		state->bLevel += 2;

		return 0;
	}
};

class CleanSpot : public Node {

	int cleaningTime = 0;
	int timer = 0;

public:
	CleanSpot(Blackboard* b, int timer) : Node(b) {
		this->timer = timer;
		cleaningTime = timer;
	}

	virtual int run() {
		int res;

		if (state->resetTimer) {
			timer = cleaningTime;
			state->resetTimer = false;
		}

		if (timer > 0) {
			if (state->dusty_spot == true) {
				cout << "DUSTY SPOT CLEAN RUNNING" << endl;
			}
			else {
				cout << "CLEAN SPOT RUNNING" << endl;
			}
			timer--;
			res = 2;
		}
		else if (state->bLevel == 0) {
			if (state->dusty_spot == true) {
				cout << "DUSTY SPOT CLEAN FAILED" << endl;
			}
			else {
				cout << "CLEAN SPOT FAILED" << endl;
			}
			res = 1;
		}
		else {
			if (state->dusty_spot == true) {
				state->dusty_spot = false;
				cout << "DUSTY SPOT CLEAN SUCCEEDED" << endl;
			}
			else {
				cout << "CLEAN SPOT SUCCEEDED" << endl;
			}
			res = 0;
		}

		if (state->bLevel > 0 && state->docked == false) {
			(state->bLevel)--;
		}
		return res;
	}

};

class DoneSpot : public Node {
public:
	DoneSpot(Blackboard* b) : Node(b) {}
	virtual int run() {

		if (state->bLevel == 0) {
			cout << "TURNING SPOT CLEANING REQUEST OFF FAILED" << endl;
			return 1;
		}

		state->spot = false;
		state->resetTimer = true;
		cout << "TURNING SPOT CLEANING REQUEST OFF SUCCEEDED" << endl;
		return 0;
	}
};

class DoneGeneral : public Node {
public:
	DoneGeneral(Blackboard* b) : Node(b) {}
	virtual int run() {
		if (state->bLevel == 0) {
			cout << "TURNING GENERAL CLEANING REQUEST OFF FAILED" << endl;
			return 1;
		}

		state->general = false;
		state->dusty_spot = false;
		cout << "TURNING GENERAL CLEANING REQUEST OFF" << endl;
		return 0;
	}
};

class Clean : public Node {
public:
	Clean(Blackboard* b) : Node(b) {}
	virtual int run() {

		if (state->bLevel == 0) {
			cout << "CLEANING FAILED" << endl;
		}
		cout << "CLEANING SUCCEEDED" << endl;
		state->bLevel -= 1;
		return 0;

	}
};

class DoNothing : public Node {
public:
	DoNothing(Blackboard* b) : Node(b) {}
	virtual int run() {
		cout << "DO NOTHING SUCCEEDED" << endl;

		return 0;
	}
};

class CheckBattery : public Node {
	bool isNegated;
public:
	CheckBattery(Blackboard* b, bool isNegated) : Node(b) 
	{
		this->isNegated = isNegated;
	}

	virtual int run() {
		int res;

		if (state->bLevel < 30) {
			state->findHomeRequested = true;
			res = 0;
		}
		else{
			res = 1;
		}

		if (isNegated == true && state->bLevel > 30) {
			state->findHomeRequested = false;
			res = 0;
		}

		return res;
	}

};

class CheckSpot : public Node {
public:
	CheckSpot(Blackboard* b) : Node(b) {}
	
	virtual int run() {
		if (state->spot == true) {
			cout << "Spot cleaning was requested" << endl;
			state->docked = false;
			return 0;
		}

		cout << "Spot cleaning was not requested" << endl;
		return 1;
	}
};

class CheckGeneral : public Node {
public:
	CheckGeneral(Blackboard* b) : Node(b) {}

	virtual int run() {
		if (state->general == true) {
			cout << "General cleaning was requested" << endl;

			state->docked = false;
			return 0;
		}

		cout << "General cleaning was not requested" << endl;

		return 1;
	}
};

class CheckDustySpot : public Node {
public:
	CheckDustySpot(Blackboard* b) : Node(b) {}

	virtual int run() {

		int dustyProbability = rand() % 100 + 1;
		if (dustyProbability <= 51 && state->dusty_spot == false){
			state->dusty_spot = true;
			cout << "Dusty spot detected by the sensor " << endl;
			return 0;
		}

		cout << "Dusty spot is not detected" << endl;

		return 1;
	}
};

#pragma once
