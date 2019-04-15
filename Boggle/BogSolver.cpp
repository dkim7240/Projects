/*
 * BogSolver.cpp : Implementation for BogSolver.h
 *
 *
 *
 *For reader's convenience (if you don't find it convenient I apologize):
 *
 *Functions....
 *
 *to read in a dictionary file and build a board: readDict(), readBoard(),
 * 						  Build_Board()
 *necessary to make a heap tree: CreateHeap(), DownHeap(), RemoveMin()
 *to remove duplicates: RemoveDuplicates(), Merge(), MergeSort()
 *
 *
 *			  <Function call tree>
 *----------------------------------------------------------------------------
 *		listWords()		listWords(int len)
 *	  	    |				|
 *	        getWords()		 getWords(int len)
 *
 *----------------------------------------------------------------------------
 *
 *		getWords(int len)      NumWords()        NumWords(int len)
 *		    |			  |			|
 *		getWords()	      getWords()	 getWords(int len)
 *
 *----------------------------------------------------------------------------
 *		 Solve()			getWords()
 *		   |				|	|
 *  Recursive---SolveIt()	Recursive---Merge()   RemoveDuplicates()
 *             	|      |		      |
 *     CreateHeap()  RemoveMin()          MergeSort()
 *	   |   		|
 *	DownHeap()   DownHeap()
 *
 *
 *   *** '|' means "it calls." (e.g. Solve() | SolveIt() means "Solve() calls
 *				SolveIt()" )
 *
 *  Created on: Nov 19, 2015
 *      Author: dkim11
 */

#include "BogSolver.h"
#include <cstdlib>

#define CAPACITY 50

using namespace std;

//Constructor
BogSolver::BogSolver()
{
	Board=NULL;
	Visited=NULL;
	WordList=NULL;
	validity=false;
}

//Destructor
BogSolver::~BogSolver()
{
	  for(int i=0; i<row; i++){
		  delete[] Board[i];
		  delete[] Visited[i];
	  }
	  delete[] Board;
	  delete[] Visited;
	  delete WordList;

}

//Build a board!
void BogSolver::Build_Board()
{
	Board=new char* [row];
	Visited=new bool* [row];

	for(int i=0; i<row; i++){
		Board[i]=new char[col];
		Visited[i]=new bool[col];
	}

}

//Reads in a dictionary file and returns true if successfully read
bool BogSolver::readDict()
{
	string word;

	while(1){
		cin>>word;
		if(word == ".") break;
		for(size_t i=0; i<word.length(); i++){
			//Capitalize the dictionary words
			if(word[i]-'a' >= 0) word[i]=(word[i]-'a')+'A';
		}
		dict.insert(word);
	}

	return true;
}


//Reads in information and build a game board (also capitalizes all letters)

bool BogSolver::readBoard()
{
	string str;
	int i=0, j=0, word_length=0, pos=0;
	cin>>row>>col;
	cin.ignore();
	if(row < 0 || col < 0) exit(1);
	Build_Board();

	//The loop to check the validity of the board based on user input
	while(getline(cin, str)){

		//Based on user's row and col value, If the user input is either
		//too much or too few to build a game board,
		//return false to show the board is invalid
		word_length+=(int)str.length();
		if(word_length>row*col) return validity;
		for(pos=0; pos<(int)str.length(); pos++){
			if(str[pos] == ' ') return validity;
			if(str[pos]-'a' >= 0) str[pos]=(str[pos]-'a')+'A';
			if(j < col){ //From this line, if input is valid start
				Board[i][j]=str[pos]; //to build a game board
				Visited[i][j]=false;
				j++;
			}else{
				j=0;
				i++;
				Board[i][j]=str[pos];
				Visited[i][j]=false;
				j++;
			}
		}
		if(word_length == row*col){ //If the game board receives all
			validity=true; // data return true for validity
			return validity; //to show the game board is valid
		}
	} return validity;
}

//Take out the top letter of MinHeap and return its information including
//its coordinates on the board

BogLett BogSolver::RemoveMin(BogWord* heap)
{
	BogLett temp;

	//In the array being used to build a MinHeap, the 0 index will not be
	//used.
	if(heap->empty() || (heap->begin()+1)->row == -1){
		temp.c='\0';
		temp.row=-1;
		temp.col=-1;
		return temp;
	}
	temp= *(heap->begin()+1);

	*(heap->begin()+1)=*(heap->end()-1);
	heap->pop_back();
	DownHeap(heap, 1);

	return temp;
}

//Function to maintain heap property after removing or inserting an element
//into a MinHeap tree

void BogSolver::DownHeap(BogWord* heap, int location)
{

	if(2*location > (int)heap->size()-1) return;
	else{
		int smallest=location;
		if(2*location <= (int)heap->size()-1 &&
				heap->at(2*location).c < heap->at(smallest).c)
			smallest=2*location;
		if(2*location+1 <= (int)heap->size()-1 &&
				heap->at(2*location+1).c < heap->at(smallest).c)
			smallest=2*location+1;

		if(smallest == location) return;

		BogLett temp=heap->at(location);
		heap->at(location)=heap->at(smallest);
		heap->at(smallest)=temp;

		DownHeap(heap, smallest);
	}

}

//Insert letters into a heap tree and make it as a MinHeap using DownHeap()
//Returns heap tree, which is a vector of BogLett (e.g. : a-> b-> c-> d-> e)

BogWord* BogSolver::CreateHeap(BogLett let, int number)
{
	BogWord* heap=new BogWord;
	BogLett new_letter;

	//initialize heap in order to make 0 index unavailable
	new_letter.c='\0';
	new_letter.col=-1;
	new_letter.row=-1;

	heap->push_back(new_letter);

	//If a heap is being created for the first time, then push all letters
	//into a heap tree
	if(number == 1){
		for(int i=0; i<row; i++){
			for(int j=0; j<col; j++){
				new_letter.c=Board[i][j];
				new_letter.row=i;
				new_letter.col=j;
				heap->push_back(new_letter);
			}
		}
	}else{
	//Push into a new heap the letters which are
	//"not visited" and adjacent to the letter taken out from initial heap
	// by RemoveMin(), which is the function parameter here BogLett let
		for(int i=let.row-1; i<=let.row+1; i++){
			for(int j=let.col-1; j<=let.col+1; j++){
				if(i == let.row && j== let.col) continue;
				if((i >= 0 && i < row) && (j >= 0 && j < col)
						&& !Visited[i][j]){
					new_letter.c=Board[i][j];
					new_letter.row=i;
					new_letter.col=j;
					heap->push_back(new_letter);
				}
			}
		}
	}

	//Maintain heap property for built heap tree
	for(int i=heap->size()-1 / 2; i >= 1; i--) DownHeap(heap, i);
	return heap;

}

//If the board is valid, search all possible words on the board given the
//dictionary file

bool BogSolver::solve() // search board for words in dict
{
	if(!validity) return false;
	WordList=new BogWordList;
	BogWord* bw=new BogWord;

	solveIt(bw, 1, "");
	delete bw;
	if(!WordList->empty()){ //Returns true if any possible word is found
		return true;
	}
	return false;

}

//As described in algorithm part of ReadMe.md, searches for words
//on all parts of the board using heap and stores them into WordList

void BogSolver::solveIt(BogWord* word, int pos, string s)
{

	BogLett le;
	BogWord* new_heap;

	if(pos > 1) le=word->back();
	new_heap=CreateHeap(le, pos); //Create a new heap for storing letters

	while(!new_heap->empty()){
		if(new_heap->size() == 1){
			new_heap->pop_back();
			continue;
		}
		le=RemoveMin(new_heap); //Take a letter out of a heap
		if(le.c == 'Q') s=s+le.c+'U'; //Handling 'Q'
		else s+=le.c; //Add the taken-out letter into a string to check
		              //if the word is valid
		word->push_back(le);

		Visited[le.row][le.col]=true;

		//Check if there can be more routes for finding valid words
		//Add the word into the list if it is both prefix and the word
		if(dict.isPrefix(s)){
			if(dict.isWord(s) && (int)s.length() > 2)
				WordList->push_back(*word);
			solveIt(word, pos+1, s);
		}
		if(!word->empty()) word->pop_back();
		s.erase(s.size()-1);
		Visited[le.row][le.col]=false;
	}
	delete new_heap;
}


//returns a copy of WordList which contains all words found

BogWordList* BogSolver::getWords()
{
	BogWordList* list=new BogWordList;

	if(validity){
		*list=*WordList;

		if(!list->empty()){
			Merge(list, 0, list->size()-1);
			RemoveDuplicates(list);
		}
	}

	return list;
}

//Remove all duplicate words after sorting

void BogSolver::RemoveDuplicates(BogWordList* word_list)
{

	BogWordList::iterator it=word_list->begin();
	BogWord::iterator bw;
	string s1="", s2="";

	//Search and remove duplicate words
	//until the iterator starting from the beginning
	//of word_list reaches the last element of word_list

	while(it!=word_list->end()-1){
		for(bw=it->begin(); bw!=it->end(); bw++) s1+=bw->c;
		for(bw=(it+1)->begin(); bw!=(it+1)->end(); bw++) s2+=bw->c;

		//If the adjacent element contains duplicate word erase it
		//Otherwise move to the next element for searching
		if(s1 == s2) word_list->erase(it+1);
		else it++;
		s1="";
		s2="";
	}

}

//Takes in a copy of WordList and then start to sort in alphabetical order
//using Merge sorting method
void BogSolver::Merge(BogWordList* bwl, size_t first, size_t last)
{
	size_t mid=(first+last)/2;

	if(first < last)	//Merge sort is based on divide and conquer.
	{
		Merge(bwl, first, mid);
		Merge(bwl, mid+1, last);
		MergeSort(bwl, first, mid, last);
	}
}

//Function necessary to merge sort
void BogSolver::MergeSort(BogWordList* bwl, size_t first, size_t mid,
		size_t last)
{
	//Divides a group into two subgroups and compare each element of
	//each subgroup. While comparing, copy the result into a temporary
	//vector and then copy this temporary vector into a copy of WordList

	BogWord::iterator i, j;
	string s1="", s2="";
	BogWordList tmp=(*bwl);
	size_t first1=first, last1=mid, first2=mid+1, last2=last;
	size_t idx=first;

	while(first1 <= last1 && first2 <= last2){
		for(i=bwl->at(first1).begin(); i!= bwl->at(first1).end();
				i++) s1+=i->c;
		for(j=bwl->at(first2).begin(); j!= bwl->at(first2).end();
				j++) s2+=j->c;

		if(s1.compare(s2) <= 0){
			tmp.at(idx++)=bwl->at(first1);
			first1++;
		}else{
			tmp.at(idx++)=bwl->at(first2);
			first2++;
		}
		s1="";
		s2="";
	}

	if(first1 > last1){
		for(size_t i=first2; i<=last2; i++) tmp.at(idx++)=bwl->at(i);
	}else{
		for(size_t i=first1; i<=last1; i++) tmp.at(idx++)=bwl->at(i);
	}
	for(size_t i=first; i<=last; i++) bwl->at(i)=tmp.at(i);

}

//Return a copy of WordList containing words of length 'len' only
BogWordList* BogSolver::getWords(int len)
{
	BogWordList* list_num=new BogWordList;
	BogWordList::iterator itr;
	BogWord::iterator it;
	string str;

	if(validity){
		list_num=getWords();
		if(!list_num->empty()){
			itr=list_num->begin();

			//If list_num contains words length not equal to 'len',
			//erase
			while(itr!=list_num->end()){
				for(it=itr->begin(); it!=itr->end(); it++)
					str+=it->c;
				if((int)str.length() != len)
					list_num->erase(itr);
				else itr++;
				str="";
			}
		}
	}

	return list_num;
}

void BogSolver::printWords()              // print all words of WordList in HBF
{
	if(validity){
		for(BogWordList::iterator i=WordList->begin();
				i!=WordList->end(); i++){
			cout<<"< ";
			for(BogWord::iterator j=i->begin(); j!=i->end(); j++){
				if(j->c == 'Q')
				cout<<j->c<<"U"<<" "
				<<j->row<<" "<<j->col<<" ";
				else cout<<j->c<<" "<<j->row<<" "<<j->col<<" ";
			}
			cout<<">"<<endl;
		}
	}
	cout<<"< >"<<endl;

}

void BogSolver::listWords()   // print just the text, no coords excluding dups.
{
	if(validity){
		BogWordList* lw=getWords();
		if(!lw->empty()){
			for(BogWordList::iterator i=lw->begin();
					i!=lw->end(); i++){
				for(BogWord::iterator j=i->begin();
						j!=i->end(); j++){
					if(j->c == 'Q') cout<<j->c<<"U";
					else cout<<j->c;
				}
				cout<<endl;
			}
			delete lw;
		}
	}
}

void BogSolver::printWords(int len)   // print len-length words in HBF. NO dups
{
	if(validity){
		BogWordList* pwl=getWords(len);
		if(!pwl->empty()){
			for(BogWordList::iterator i=pwl->begin();
					i!=pwl->end(); i++){
				cout<<"< ";
				for(BogWord::iterator j=i->begin();
						j!=i->end(); j++){
					if(j->c == 'Q')
						cout<<j->c<<"U"<<" "
						<<j->row<<" "<<j->col<<" ";
					else cout<<j->c<<" "<<j->row<<" "
							<<j->col<<" ";
				}
			cout<<">"<<endl;
			}
			delete pwl;
		}
	}
	cout<<"< >"<<endl;

}

void BogSolver::listWords(int len) // just the text of len-length words. No dups
{
	if(validity){
		BogWordList* lwl=getWords(len);
		if(!lwl->empty()){
			for(BogWordList::iterator i=lwl->begin();
					i!=lwl->end(); i++){
				for(BogWord::iterator j=i->begin(); j!=i->end();
						j++){
					if(j->c == 'Q') cout<<j->c<<"U";
					else cout<<j->c;
				}
				cout<<endl;
			}
		delete lwl;
		}
	}
}

int  BogSolver::numWords()        // returns number of words found
{
	int count=0;

	if(validity){
		BogWordList* nw=getWords();

		if(!nw->empty()){
			for(BogWordList::iterator i=nw->begin();
					i!=nw->end(); i++)
				count++;
			delete nw;
		}
	}
	return count;
}
int  BogSolver::numWords(int len)  // number of words of length len
{
	int count=0;

	if(validity){
		BogWordList* nwl=getWords(len);
		if(!nwl->empty()){
			for(BogWordList::iterator i=nwl->begin();i!=nwl->end();
					i++)
				count++;
			delete nwl;
		}
	}
	return count;
}

