#include "stdafx.h"
#include "ANN.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

int main()
{
	ifstream myfile;
	row r;
	string classes[CLASSES] = { "Iris-versicolor", "Iris-setosa", "Iris-virginica" };
	string line;
	double sl, sw, pl, pw;
	char cont;
	
	ANN* ann = new ANN();

	myfile.open("PS4__Iris_data.txt");

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			
			replace(line.begin(), line.end(), ',', ' ');
			istringstream ss(line);

			ss >> r.sepal_length >> r.sepal_width >> r.petal_length >> r.petal_width >> r.type;

			if (r.type == "Iris-versicolor") {
				r.category[0] = 1;
				r.category[1] = 0;
				r.category[2] = 0;
			}
			else if (r.type == "Iris-setosa") {
				r.category[0] = 0;
				r.category[1] = 1;
				r.category[2] = 0;
			}
			else {
				r.category[0] = 0;
				r.category[1] = 0;
				r.category[2] = 1;
			}

			r.isSelected = false;

			ann->fillInputNodes(r);
		}

		myfile.close();
	}

	ann->trainANN();
	ann->printTestSet();

	while (1) {
		cout << "Enter each value for 1) Sepal Length, 2) Sepal Width, 3) Petal Length, and 4) Petal Width" << endl;
		cin >> sl >> sw >> pl >> pw;
		cin.get(cont);

		cout << "-------------------------" << endl;
		cout << "Expected class: " << endl;
		cout << ann->forwardPass(sl, sw, pl, pw) << endl;

		cout << "Would you like to continue ? (Y / N)" << endl;
		cin.get(cont);
		
		if (cont == 'N' || cont == 'n') break;
	}

	delete ann;
	ann = NULL;

	return 0;
}
