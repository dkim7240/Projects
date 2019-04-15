#ifndef BOGSOLVER_H
#define BOGSOLVER_H

#include "BogWordList.h"
#include "Dictionary.h"

//
// A class to solve boggle puzzles
//  The class reads in a dictionary and a board
//  Then solves it.
//  Once it has solved it, the solver can be
//  asked for the words it found, can be asked to print the words
//  with their locations, and to print the words just as strings
//

class BogSolver
{
    public:
        BogSolver();
        ~BogSolver();
        bool readDict();
        bool readBoard();

        BogWordList *getWords(); // returns all words found
        BogWordList *getWords(int len); // returns words of len len
        bool solve();                   // search board for words in dict

        int  numWords();                // returns number of words found
        int  numWords(int len);         // number of words of length len
        void printWords();              // print all words in HBF
        void printWords(int len);       // print len-length words in HBF
        void listWords();               // print just the text, no coords
        void listWords(int len);        // just the text, no coords

   private:
        void Build_Board();
        void RemoveDuplicates(BogWordList* word_list); //Remove duplicates
        					       //from a copy of WordList
        void Merge(BogWordList* bwl, size_t first, size_t last);
        void MergeSort(BogWordList* bwl, size_t first, size_t mid, size_t last);
        void solveIt(BogWord* word, int pos, string s); //Another solve()
        BogWordList* WordList; //An original list of all possible words on a
                               //board
        BogLett RemoveMin(BogWord* heap);  // Remove the top element of MinHeap
        BogWord* CreateHeap(BogLett let,int number); //Make a vector for storing
                                                     //letters
        void DownHeap(BogWord* heap, int location);
        Dictionary dict;                // must use a Dictionary
        char** Board;
        bool** Visited;	     //Check whether a letter on the board was visited
        int row, col;
        bool validity;	     //Check board's validity


};
#endif
