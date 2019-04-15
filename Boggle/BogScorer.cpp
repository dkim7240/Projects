/*
 * BogScorer.cpp : Implementation for BogScorer.h
 *  Created on: Nov 30, 2015
 *      Author: dkim11
 */

#include "BogScorer.h"
#include <iostream>

using namespace std;

BogScorer::BogScorer()
{
	validity="";
	word="";
	score=0;
	num_total=0;
}

BogScorer::~BogScorer()
{

}

//Reads in the validity result of user's words
void BogScorer::readAnswers()
{
	string s;
	int pos=0;
	bool space=false;

	while(getline(cin, s)){
		while(s[pos] != '\0'){
			if(s[pos] == ' '){
				space=true;
				pos++;
				continue;
			}
			if(!space) validity+=s[pos];
			else word+=s[pos];
			pos++;
		}
		scoreAnswers();
		space=false;
		pos=0;
	}
}

//Checks score for user's each valid word based on official Boggle rules
void BogScorer::scoreAnswers()
{
	int ws=0;

	if(validity == "OK"){
		if(word.length() == 3 || word.length() == 4){
			ws=1;
			score+=1;
		}
		else if(word.length() == 5){
			ws=2;
			score+=2;
		}
		else if(word.length() == 6){
			ws=3;
			score+=3;
		}
		else if(word.length() == 7){
			ws=5;
			score+=5;
		}
		else if(word.length() >= 8){
			ws=11;
			score+=11;
		}
		num_total++;
		cout<<ws<<" OK "<<word<<endl;
	}else cout<<ws<<" NO "<<word<<endl;

	validity="";
	word="";
}

//Prints out total number of valid user's words and sum of the scores
void BogScorer::printAnswers()
{
	cout<<num_total<<" words "<<score<<" points"<<endl;

}
