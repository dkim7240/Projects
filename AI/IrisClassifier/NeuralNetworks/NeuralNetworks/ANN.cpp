#include "stdafx.h"
#include "ANN.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;

/* Set up random initial weights */

ANN::ANN()
{
	double random;
	srand(time(NULL));

	for (int i = 0; i < total_nodes; i++) {
		for (int j = 0; j < total_nodes; j++) {
			weights[i][j] = -2;

			
			/* Set up initial weights that connect nodes in each layer */
			
			if (i != j && (i < INPUTS && (j >= INPUTS && j < INPUTS + HIDDEN_NEURONS)) || 
				((i >= INPUTS && i < INPUTS + HIDDEN_NEURONS) && 
				(j >= INPUTS + HIDDEN_NEURONS && j < INPUTS + HIDDEN_NEURONS + OUTPUTS)) ||
				(i >= INPUTS + HIDDEN_NEURONS + OUTPUTS && 
				 j >= INPUTS && j < INPUTS + HIDDEN_NEURONS + OUTPUTS)){
				random = (((double)rand()) / (double)RAND_MAX) * 1 - 0.5;
				weights[i][j] = random;
			}
			
		}
		neurons[i] = 0;

	}

	/* Set up values for bias neurons */
	for (int k = 0; k < BIAS; k++) {
		neurons[INPUTS + HIDDEN_NEURONS + OUTPUTS + k] = 1.0;
	}
	
}

ANN::~ANN()
{
	delete train;
    delete test;
	delete validate;

	train = test = validate = NULL;
	
}

void ANN::fillInputNodes(row r)
{
	rows.push_back(r);
}

/*Activation function used: Sigmoid function */

std::string ANN::forwardPass(double sl, double sw, double pl, double pw)
{
	double potential = 0, activation_fn = 0;
	string result = "";
	int i, j;
	
	neurons[0] = pl;
	neurons[1] = pw;
	neurons[2] = sl;
	neurons[3] = sw;

	/* Calculate potential of each neuron and 
	   then plug that result into each activation function */

	for (i = INPUTS; i < total_nodes - 1; i++) {
		for (j = 0; j < total_nodes; j++) {
			if (weights[j][i] != -2) {
				potential += (neurons[j] * weights[j][i]);
			}
		}

		potentials[i - INPUTS] = potential;
		activation_fn = 1 / (1 + exp(-potential));
		neurons[i] = activation_fn;
		potential = 0;
	}

	for (i = INPUTS + HIDDEN_NEURONS; i < total_nodes - 1; i++) {
		if (neurons[i] <= 0.5) {
			result += '0';
		}
		else {
			result += '1';
		}
	}

	if (result == "100") result = "Iris-versicolor";
	else if (result == "010") result = "Iris-setosa";
	else if (result == "001") result = "Iris-virginica";
	else result = "unknown";

	return result;

}

double ANN::backwardPass(int dataIdx)
{
	int i, j, idx = 0;
	double product_delta_weight = 0, learning_rate = 0.1;
	double deltas[HIDDEN_NEURONS + OUTPUTS];
	double total_error;

		/* 1. The calculation of error is based on difference 
				between target and actual output */

		total_error = 0;
		
		for (i = INPUTS + HIDDEN_NEURONS; i < total_nodes - 1; i++) {
			deltas[HIDDEN_NEURONS + idx] = (neurons[i] * (1.0 - neurons[i])) *
				(rows[dataIdx].category[idx] - neurons[i]);
			
			total_error += (pow(rows[dataIdx].category[idx] - neurons[i], 2.0));
			idx++;
		}
		idx = 0;

		total_error = total_error * 0.5;


		/* 2. Calculate the contribution to the error by the hidden
			  neuron. */

		for (i = INPUTS; i < INPUTS + HIDDEN_NEURONS; i++) {
			deltas[i - INPUTS] = (neurons[i] * (1 - neurons[i]));
			for (j = INPUTS + HIDDEN_NEURONS; j < total_nodes - 1; j++) {
				product_delta_weight += (weights[i][j] * deltas[j - INPUTS]);
			}
			deltas[i - INPUTS] = deltas[i - INPUTS] * product_delta_weight;
			product_delta_weight = 0;

		}

		/* 3. Get the rate of change of the error which is the important feedback
			  through the network and update each weight. */

		for (int i = 0; i < total_nodes; i++) {
			for (int j = 0; j < total_nodes; j++) {
				if (weights[i][j] != -2) {
					weights[i][j] += (learning_rate * neurons[i] * deltas[j - INPUTS]);
				}
			}
		}
	
		return total_error;
}

void ANN::trainANN()
{
	string modelResult;
	double accuracy = 0, errorRate = 1.0, minimumErrorRate = 100;
	int cnt = 0;

	partition = rows.size() / MULTIFOLD;
	training = rows.size() * TRAINING;

	test = new int[TEST * rows.size()];
	train = new int[TRAINING * rows.size()];
	validate = new int[VALIDATE * rows.size()];

	splitData(0);

	// Adjust the weight until each row of the training set
	// results the total error less than the threshold value.

	while (cnt < training) {
		while (errorRate > threshold) {
			forwardPass(rows[train[cnt]].sepal_length,
				rows[train[cnt]].sepal_width,
				rows[train[cnt]].petal_length,
				rows[train[cnt]].petal_width);
			errorRate = backwardPass(train[cnt]);
		}
		
		errorRate = 1.0;
		cnt++;
	}

	for (int j = 0; j < VALIDATE * rows.size(); j++) {
		forwardPass(rows[validate[j]].sepal_length,
			rows[validate[j]].sepal_width,
			rows[validate[j]].petal_length,
			rows[validate[j]].petal_width);
			backwardPass(validate[j]);
	}


}

void ANN::printTestSet()
{
	cout << "---Test Set Data (Sepal Length, Sepal Width, Petal Length, Petal Width)--- "
		 << endl;
	for (int i = 0; i < TEST * rows.size(); i++) {
		cout << rows[test[i]].sepal_length << " " << rows[test[i]].sepal_width << " "
			<< rows[test[i]].petal_length << " " << rows[test[i]].petal_width << " "
			<< rows[test[i]].type << endl;
	}
}

/* Splitting the data into training set (0.6)
						   validate set (0.2) 
						   test set (0.2) */

void ANN::splitData(int fold)
{
	int pos, random, count = 0;

	while(count < (TRAINING * rows.size())){
		random = rand() % rows.size();
		if (rows[random].isSelected == false) {
			rows[random].isSelected = true;
			train[count++] = random;
		}
	}

	count = 0;
	while (count < (VALIDATE * rows.size())) {
		random = rand() % rows.size();
		if (rows[random].isSelected == false) {
			rows[random].isSelected = true;
			validate[count++] = random;
		}
	}

	count = 0;
	while (count < (TEST * rows.size())) {
		random = rand() % rows.size();
		if (rows[random].isSelected == false) {
			rows[random].isSelected = true;
			test[count++] = random;
		}
	}

}

