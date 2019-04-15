// By Dokyun Kim (dkim11)

#include "Astar.h"
#include <iostream>
#include <algorithm>


/* A* constructor that initializes the start state,
   the goal state, the frontier, and the visited node list
   Parameter: the starting pancake stack from the user  */

Astar::Astar(vector<int> &s)
{
	start = new Node();
	start->stack = s;
	start->fg = 0;
	start->fh = heuristic(s);
	start->parent = NULL;
	start->child = NULL;
	start->position = -1;

	goal = new Node();
	goal->stack = { 5, 4, 3, 2, 1 };
	goal->fg = 0;
	goal->fh = 0;
	goal->parent = NULL;
	goal->child = NULL;
	goal->position = -1;

	frontier.push(start);

	visited = new Node();
	*visited = *start;

}

Astar::~Astar()
{
	Node* visitedItr = visited;
	
	while (visitedItr != NULL) {
		Node* temp = visitedItr->parent;
		delete visitedItr;
		visitedItr = temp;
	}

	delete start;

	if (start != goal) delete goal;
	start = goal = NULL;
}


void Astar::search()
{
	Node* result = searchA(start);
	if (result != NULL) print(result, 1);
}

/* Prints out the solution to reach the goal state 
   and returns the total number of steps of the solution
   Parameters: The node of the start state and the 
               step number in the solution */

int Astar::print(Node* current, int cnt)
{
	int step;

	if (current == start) {
		for (int i = 0; i < SIZE; i++) {
			cout << current->stack[i] << " ";
		}
		cout << "     ";
		if (current->position != -1) {
			cout << "1) Insert spatula into position #" 
				<< current->position + 1 << endl << endl;
		}
		return cnt;
	}

	step = print(current->parent, cnt + 1);

	for (int i = 0; i < SIZE; i++) {
		cout << current->stack[i] << " ";
	}
	cout << "     ";

	if (current != goal) {
		cout << step - cnt + 1
			<< ") Insert spatula into position #"
			<< current->position + 1 << endl << endl;
		
		delete current;
		current = NULL;
	}

	return step;
	
}


/*  Heuristic function that returns the number of gaps
    in pancake stack
	Parameter: The pancake stack */

int Astar::heuristic(vector<int> &val)
{
	int gap = 0;
	for (int i = 0; i < val.size() - 1; i++) {
		if (abs(val[i + 1] - val[i]) > 1) gap++;
	}

	return gap;

}

/* The core method that executes A* algorithm
   which returns the node of the goal state if the
   solution is found (otherwise NULL) 
Parameter: The node of starting state */

Node* Astar::searchA(Node* node)
{
	if (frontier.empty()) {
		cout << "No solution found" << endl;
		return NULL;
	}
	Node* minf = frontier.top();
	node->position = minf->position;

	if (minf != start) {
		minf->parent = node;
		minf->fg = node->fg + node->fh;
	}

	if (frontier.isEqual(minf, goal->stack)) {
		frontier.pop();
		minf->position = -1;
		goal = minf;
		return minf;
	}

	// expand the node

	Node temp = *minf;
	frontier.pop();

	for (int pos = 0; pos < SIZE - 1; pos++) {
		selectChild(&temp, pos);
	}
	
	return searchA(minf);
}


/* The method to expand the selected node from the 
   frontier.
Parameters: The selected node and each position in the 
            pancake stack where the spatula will be inserted
			to flip the pancakes */

void Astar::selectChild(Node * node, int pos)
{
	vector<int> childState = node->stack;
	
	reverse(childState.begin() + pos, childState.end());
	
	Node* childNode = new Node();
	childNode->stack = childState;
	childNode->parent = node;
	childNode->fh = heuristic(childState);
	childNode->fg = node->fg + (SIZE - pos);
	childNode->position = pos;

	Node* childFound = frontier.find(childState);

	if (childFound == NULL || visitedCheck(childNode) == false) {
		
		frontier.push(childNode);
		return;
	}

	else if (childFound != NULL) {
		if (childNode->fg + childNode->fh < childFound->fg + childFound->fh) {
			delete childFound;
			frontier.push(childNode);
			return;
		}
	}

	delete childNode;
}

/* The method to check whether a given child node from the expansion above
   has already been visited. It returns true if the node has been visited
   and otherwise false.

Parameter: A child node of the selected node from the frontier */

bool Astar::visitedCheck(Node * node)
{
	bool isVisited = false;

	Node* visitedItr = visited;
	Node* temp = NULL;

	while (visitedItr != NULL) {

		if (isVisited) break;

		for (int idx = 0; idx < visitedItr->stack.size(); idx++) {
			if (visitedItr->stack[idx] != node->stack[idx]) {
				isVisited = false;
				break;
			}
			isVisited = true;
		}

		temp = visitedItr;
		visitedItr = visitedItr->parent;
		
	}
	
	if (isVisited == false) {
		visitedItr = new Node();
		*visitedItr = *node;
		temp->parent = visitedItr;
	}

	return isVisited;
}
