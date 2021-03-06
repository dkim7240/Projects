#include "stdafx.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <iterator>
#include <algorithm>
#include <functional>
#include <cmath>
#include <time.h>

using namespace std;

#define POPULATION 8
#define BOX_NUMBER 7
#define WEIGHT_LIMIT 120


/*
chromosomeGenerator() generates the random population.

Parameters: 
1) The hash table to check whether a randomly
   generated box selection already exists
2) Empty list(vector here) of box selection

*/

void chromosomeGenerator(map<string, float>& chromosomeTable,
	vector<pair<string, float>>& chromosomes)
{
	int i, j;
	char* chromosome;

	// Initialize random seed
	srand(time(NULL));

	// Generating random chromosomes
	for (i = 0; i < POPULATION; i++) {

		chromosome = new char[BOX_NUMBER + 1];

		for (j = 0; j < BOX_NUMBER; j++) {
			chromosome[j] = (rand() % 2) + '0';
		}

		chromosome[BOX_NUMBER] = NULL;

		/*	Check if the chromosome generated above was not found in the record
		Table */

		if (chromosomeTable.find(string(chromosome)) == chromosomeTable.end())
		{
			chromosomeTable.insert(pair <string, float>(string(chromosome),
				1.0));
			chromosomes.push_back(pair <string, float>(string(chromosome),
				1.0));
		}
		else {
			delete[] chromosome;
			chromosome = NULL;
			i--;
			continue;
		}

		delete[] chromosome;
		chromosome = NULL;

	}
}

/*
fitnessTest() is the fitness function which calculates 
each total value of each permutation and returns
the sum of the values of each chromosome.

Parameters:
1) weights[] and values[] that store weight and value
   of each box
2) Empty list(vector here) of box selection

*/


int fitnessTest(int* weights, int* values, 
	vector<pair<string, float>>& chromosomes)
{
	vector<pair<string, float>>::iterator itr;
	int cumulative_value = 0, total_weight = 0, total_value = 0, cnt = 0;

	for (itr = chromosomes.begin(); itr != chromosomes.end(); ++itr) {
		for (int idx = 0; idx < BOX_NUMBER; idx++) {
			if (itr->first[idx] == '1') {
				total_value += values[idx];
				total_weight += weights[idx];
			}
		}

		if (total_weight <= WEIGHT_LIMIT) itr->second = total_value;
		else {
			total_value = 0;
			itr->second = total_value;
		}
		cumulative_value += total_value;

		total_weight = total_value = 0;

	}

	return cumulative_value;
}

bool sort_by(const pair<string, float>&a, const pair<string, float>&b)
{
	return a.second > b.second;
}

/* In crossover(), Matings among the chromosomes occur in order to 
   produce the new children chromosomes. First, the 
   least fit chromosomes, 50% of the population, are culled. Then,
   each pair of chromosomes produces two children chromosomes by
   swapping the tails of their two parents. 
   
   Parameters: 1) The sum of total values of chromosomes and 
               2) the list of chromosomes
   */

void crossover(float cumulative_value, vector<pair<string, float>>& chromosomes)
{
	int crossoverIdx, limit = 0, limit2 = 0;
	string child = "";

	for (int i = 0; i < POPULATION; i++)
	{
		if (cumulative_value != 0) {
			chromosomes[i].second = chromosomes[i].second / (float)cumulative_value;
		}	
	}
	sort(chromosomes.begin(), chromosomes.end(), sort_by);
	chromosomes.erase(chromosomes.begin() + (POPULATION / 2), 
		chromosomes.end());

	for (int i = 0; i < (POPULATION / 2) - 1; i += 2) {
		for (int j = 1; j < (POPULATION / 2); j += 2) {

			crossoverIdx = rand() % (POPULATION - 1);
			child.append(chromosomes[i].first.begin(), 
				chromosomes[i].first.begin() + crossoverIdx + 1);
			child.append(chromosomes[j].first.begin() + crossoverIdx + 1, 
				chromosomes[j].first.end());
			chromosomes.push_back(pair <string, float>(child, 1.0));
			child = "";
			child.append(chromosomes[j].first.begin(), 
				chromosomes[j].first.begin() + crossoverIdx + 1);
			child.append(chromosomes[i].first.begin() + crossoverIdx + 1, 
				chromosomes[i].first.end());
			chromosomes.push_back(pair <string, float>(child, 1.0));
			child = "";
		}
	}

	chromosomes.erase(chromosomes.begin(), chromosomes.begin() + (POPULATION / 2));

}

/*
   mutation() takes the list of chromosomes as its parameter 
   and does the following:

   1) The mutation probability is set to 0.6. Mutation occurs 
      in a chromosome if any randomly generated number is between 0 and 60.

   2) Pick a random point for mutation. 
	  If the selected gene of a chromosome is 0, flip it to 1 and vice versa. */

void mutation(vector<pair<string, float>> &chromosomes)
{
	int mutationProb, mutationIdx;
	string key;

	for (int i = 0; i < POPULATION; i++)
	{
		mutationProb = rand() % 100 + 1;

		if (mutationProb <= 60) {
			mutationIdx = rand() % BOX_NUMBER;
			key = chromosomes[i].first;

			if (key[mutationIdx] == '1') 
				chromosomes[i].first[mutationIdx] = '0';
			else if (key[mutationIdx] == '0') 
				chromosomes[i].first[mutationIdx] = '1';
		}

	}

}

/* Compare the current set of chromosomes with the chromosomes
   in the solution set. If some new chromosomes have higher 
   fitness scores, they will replace the chromosomes in
   the solution set. If equal, they will be added to the 
   solution set. Otherwise, nothing happens. */

void solutionTest(vector<pair<string, float>>& solutions, 
	vector<pair<string, float>>& chromosomes)
{
	bool isNew = true;

	for (auto itr = chromosomes.begin();
		itr != chromosomes.end(); itr++) {
		if (solutions.begin()->second < itr->second) {
			solutions.clear();
			solutions.push_back(pair <string, float>(itr->first,
				itr->second));
		}
		else if (solutions.begin()->second == itr->second) {
			for (int i = 0; i < solutions.size(); i++) {
				if (solutions[i].first == itr->first) {
					isNew = false;
				}
			}

			if (isNew) {
				solutions.push_back(pair <string, float>(itr->first,
					itr->second));
			}
		}
		isNew = true;
	}
}




int main()
{
	int weights[BOX_NUMBER] = { 20, 30, 60, 90, 50, 70, 30 };
	int values[BOX_NUMBER] = { 6, 5, 8, 7, 6, 9, 4 };
	int cnt = 0;
	float cval = 0;

	vector<pair<string, float>> solutions;
	map<string, float> chromosomeTable;
	vector<pair<string, float>> chromosomes;

	solutions.push_back(pair <string, float>("0000000", 0));
	chromosomeGenerator(chromosomeTable, chromosomes);
	cval = fitnessTest(weights, values, chromosomes);
	
	while (cnt < (pow(2, BOX_NUMBER) / POPULATION)) {

		crossover(cval, chromosomes);
		mutation(chromosomes);
		cval = fitnessTest(weights, values, chromosomes);
		solutionTest(solutions, chromosomes);
		cnt++;
	}

	cout << "Solution(s): " << endl;
	for (int i = 0; i < solutions.size(); i++) {
		for (int j = 0; j < BOX_NUMBER; j++) {
			if (solutions[i].first[j] == '1') {
				cout << j + 1 << "th box with the weight " << weights[j]
					<< " and the value " << values[j] << endl;
			}
		}

		cout << "Total value: " << solutions[i].second << endl << endl;
	}

	return 0;
}



