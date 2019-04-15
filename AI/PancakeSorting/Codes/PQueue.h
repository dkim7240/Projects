// By Dokyun Kim (dkim11)

#pragma once

#ifndef PQUEUE_H
#define PQUEUE_H

#include <queue>
#include "Node.h"
#include <functional>

/* A customized priority queue module for the frontier in A* algorithm
   which includes the search method (find()) to find a certain node in 
   the priority queue and the compare method(isEqual()) which returns
   true if a node contains the given pancake configuration */

class Compare
{
public:
	bool operator()(Node* a, Node* b)
	{
		return (a->fh + a->fg) > (b->fh + a->fg);
	}
};

class PQueue : public std::priority_queue<Node*, vector<Node*>, Compare>
{

public:
	bool isEqual(Node* lhs, vector<int> &s)
	{
		vector<int>::iterator it;
		int idx = 0;
		for (it = lhs->stack.begin(); it != lhs->stack.end(); it++) {
			if (*it != s[idx]) return false;
			idx++;
		}

		return true;
	}

    Node* find(vector<int> val) 
	{
		auto first = this->c.cbegin();
		auto last = this->c.cend();
		while (first != last) {
			if (isEqual(*first , val)) return *first;
			++first;
		}
		return NULL;
	}

	PQueue() {

	}
	
};


#endif

