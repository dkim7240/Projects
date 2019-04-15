// By Dokyun Kim (dkim11)

#include <iostream>
#include "PQueue.h"
#include "Astar.h"

using namespace std;

int main()
{

	int pancake;
	vector<int> pancakes;
	Astar* astar;

	cout << "Enter the pancake sizes (1 ~ 5): " << endl;


	for (int i = 0; i < SIZE; i++) {
		cin >> pancake;
		if (pancake < 6) {
			pancakes.push_back(pancake);
		}
		else {
			cout << "The size is out of range! Exiting the program... " << endl;
			return 1;
		}
	}

	astar = new Astar(pancakes);
	astar->search();

	delete astar;
	astar = NULL;

//	system("pause");

	return 0;

}