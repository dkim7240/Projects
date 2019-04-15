/*
 * provides user's final score based on
 * number of answers and score for each answer
 */

#ifndef BOGSCORER_H
#define BOGSCORER_H

#include <string>

using namespace std;

class BogScorer
{
public:
		BogScorer();
		~BogScorer();
		void readAnswers(); // Reads in report from BogChecker or
		                    // BogValidator
		void printAnswers(); // Prints out the number of
		                     // user's valid words and the total score
		                     // earned

private:

	       string validity, word;
	       void scoreAnswers(); // Assign score for each valid word
	       int score;
	       int num_total;

};

#endif
