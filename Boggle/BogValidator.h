/*
 * Reads in the dictionary file and the game board. Then,
 * checks validity of user's words and reports the result
 */

#ifndef BOGVALIDATOR_H
#define BOGVALIDATOR_H

#define ALPHABET 26

#include <string>
#include "Dictionary.h"
#include "BogWordList.h"

class BogValidator
{
public:
        BogValidator();
        ~BogValidator();
        bool readDict();                // read in a dictionary
        bool readBoard();               // read in a board
        bool isValid(std::string s);    // validates one word
        void checkWords();              // validates cin.  Goes to EOF.

private:
        Dictionary dict;                // must use a Dictionary
        Dictionary UserWord;
        BogWord al[ALPHABET];	//  An alphabetical array to store each
         	 	 	        //  letter on the board
        char** Board;
        int row, col;

};

#endif

