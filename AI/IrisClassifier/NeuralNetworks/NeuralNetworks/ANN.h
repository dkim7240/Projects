#ifndef _ANN_H
#define _ANN_H

#include <string>
#include <vector>

#define INPUTS 4
#define OUTPUTS 3
#define HIDDEN_NEURONS 2
#define BIAS 1
#define CLASSES 3
#define TRAINING 0.6
#define VALIDATE 0.2
#define TEST 0.2
#define MULTIFOLD 5

struct row {
	double sepal_length;
	double sepal_width;
	double petal_length;
	double petal_width;
	std::string type;
	int category[CLASSES];
	bool isSelected;

};

class ANN
{
	int total_nodes = INPUTS + HIDDEN_NEURONS + OUTPUTS + BIAS;
	double weights[INPUTS + HIDDEN_NEURONS + OUTPUTS + BIAS][INPUTS + HIDDEN_NEURONS + OUTPUTS + BIAS];
	double threshold = 0.05;
	double neurons[INPUTS + HIDDEN_NEURONS + OUTPUTS + BIAS];
	double potentials[HIDDEN_NEURONS + OUTPUTS];
	int* train;
	int* validate;
	int* test;
	int partition, training;
	std::vector<row> rows;

public:

	ANN();
	~ANN();
	void fillInputNodes(row r);
	std::string forwardPass(double sl, double sw, double pl, double pw);
	double backwardPass(int dataIdx);
	void splitData(int fold);
	void trainANN();
	void printTestSet();
	

};


#endif // !_ANN_H
