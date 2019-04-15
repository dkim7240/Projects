/*
 * BogValidator.cpp : Implementation for BogValidator.h
 *
 *  Created on: Nov 28, 2015
 *      Author: dkim11
 */

#include <iostream>
#include "BogValidator.h"

using namespace std;

BogValidator::BogValidator()
{
	Board=NULL;

}

BogValidator::~BogValidator()
{

	  for(int i=0; i<row; i++) delete[] Board[i];
	  delete[] Board;

}

//Reads in a dictionary file and returns true if successfully read
bool BogValidator::readDict()
{
	string word;

	while(1){
		cin>>word;
		if(word == ".") break;
		for(size_t i=0; i<word.length(); i++){
			//Capitalize the letters of dictionary words
			if(word[i]-'a' >= 0) word[i]=(word[i]-'a')+'A';
		}
		dict.insert(word);
	}

	return true;
}

//Make a board based on user input and returns true if a board is built
//successfully

bool BogValidator::readBoard()
{
	BogLett ch;
	cin>>row>>col;

	if(row <= 0 || col <= 0) return false;

	Board=new char* [row];

	for(int i=0; i<row; i++) Board[i]=new char[col];
	for(int i=0; i<row; i++){
		for(int j=0; j<col; j++){
			cin>>Board[i][j];
			if(Board[i][j]-'a' >= 0)
				Board[i][j]=(Board[i][j]-'a')+'A';
			ch.c=Board[i][j];
			ch.row=i;
			ch.col=j;

			//Store each letter on the board in an alphabetical
			//array (e.g. 'A' is saved into al[0], 'B'
			//             is saved into al[1])

			al[Board[i][j]-'A'].push_back(ch);
		}
	}

	return true;

}

// Returns true if a user word is valid. If not, false
bool BogValidator::isValid(string s)
{
	BogWord::iterator i, j;
	bool valid=false, cont=false;
	int r_c, c_c;
	unsigned long pos=0;

	//First, check if a user's word can be found in dictionary and its
	//length is more than 2
	if(dict.isWord(s) && (int)s.length() > 2){
		while(pos < s.length()-1){

		//Check a letter and its next letter of a user's word and
	        //compare their coordinates. In this function, two letters are
	        //adjacent on the board if their x-coordinates and y-coordinates
		//do not differ more than absolute value of 1.
		    for(i=al[s[pos]-'A'].begin();i!=al[s[pos]-'A'].end(); i++){
			    for(j=al[s[pos+1]-'A'].begin();
					    j!=al[s[pos+1]-'A'].end(); j++){
					r_c=(j->row)-(i->row);
					c_c=(j->col)-(i->col);
					if(r_c <= 1 && r_c >= -1 &&
					   c_c <= 1 && c_c >= -1){
						cont=true;
						break;
					}
				}
				if(cont){  // If any two letters' coordinates
					pos++; //are adjacent, move to the next
					break; //letter and repeat comparing
				}
			}
			if(!cont) break;    //If any two letters' coordinates
			cont=false;	    //differ more than |1|, then a
		}			    //user's word is invalid
		if(pos == s.length()-1) valid=true; //if all letters pass
	}				            //adjacency condition,
	return valid;				    //the user word is valid
}

// validates cin.  Goes to EOF.
void BogValidator::checkWords()
{
	string w;

	while(cin>>w){
		//Prints out the result of each user word
		if(isValid(w) && !UserWord.isWord(w)){
			UserWord.insert(w);
			cout<<"OK "<<w<<endl;
		}else if((isValid(w) && UserWord.isWord(w)) || !isValid(w)){
			cout<<"NO "<<w<<endl;
		}
	}
}
