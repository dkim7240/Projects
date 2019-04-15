// By Dokyun Kim (dkim11)

#pragma once

#ifndef NODE_H
#define NODE_H

#include <vector>

using namespace std;

class Node
{
public:
	vector<int> stack;
	int fg, fh, position;
	Node* parent;
	Node* child;

	

};

#endif