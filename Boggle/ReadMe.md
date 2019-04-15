# ReadMe for Boggle Program by Dokyun Kim (dkim11)

<WARNING>
--It is your(client's) responsibility to free memory allocated by getWords() if
  you want to make any other functions  using getWords() except already 
  implemented functions!!!!!!!!!!!!!!!!!!!!!!!
  

1.  The purpose of your program
	--This program is built to provide a solution, check
	  user's answers are correct, and check user's scores
	  for a Boggle game.

2.  List of files with one/or two-line summary
	--BogSolver.cpp, BogSolver.h, solverMain.cpp
	--BogValidator.cpp, BogValidator.h, checkerMain.cpp
	--Dictionary.cpp, Dictionary.h, 
	--Node.cpp, Node.h
	--BogWordList.h
	--BogScorer.cpp, BogScorer.h, scorerMain.cpp
	
3.  How to compile it (saying "use included Makefile" is ok)

	--Use included Makefile
	
4.  Outline of data structure
	
	--Tries (already built-in) for dictionary
	--MinHeap for searching all possible solutions (words) for a Boggle game
	
5.  Outline of algorithm

	<BogSolver>
	--solve(): calls a private function solveIt() and returns true
		   if any solution is found for the game  
		
	--solveIt(): Find all possible words (including coordinates)
		1) Declare a string
		2) make a MinHeap and store all "unvisited" letters of the 
		   board into MinHeap 	    
		3) Use RemoveMin() to remove a letter from MinHeap
		4) Add the letter from 3) into a word string   
		5) Mark the letter from 3) on the board as "visited"
		6) Check if the string is a valid prefix based on dictionary
		7) If the string is also a word and has more than 2 letters,
		   then add it to WordList which stores solution
		8) If not but still a prefix then repeat from step 2)
		9) If the string is not even a prefix, take the letter out of
		   the string and mark that letter as "unvisited" on the board
		10) Repeat from 2) to 9) until all letters are taken out from
		    a heap      
	
	--Removing duplicates for listWords(), listWords(int len), numWords(),
	  numWords(int len), getWords(), and getWords(int len):
	  
	  1) Get a copy of WordList first! (We don't want our original WordList
	  			            to be modified!!!)
	  2) Use merge sort to sort all the words stored in WordList
	     in alphabetical order
	  3) From the beginning of our copied list, if its next element is
	     a duplicate of current element, remove the next element.
	     If not, move to the next index of the list.
	  4) Repeat 3) until the current element is the end of the list     
	  
	  *This removing procedure will have O(n + n log n) so O(n log n).
	  		 
	<BogValidator>
	
	--isValid(string s): Read in a word from the user and check if it counts
	                     as a solution
	  1) Make an alphabetic array of size 26 and store each letter on the 
	     board into each alphabetic index of the array (For example,a = '0')     
	  2) From the beginning of s, check the letter and if it is a letter on
	     the board.
	  3) Check its next letter. If it is also a letter on the board and 
	     adjacent to the letter in 2), repeat from 2) until all letters 
	     of the string are explored except that the
	     position changes, not the beginning. 
	
	<BogScorer>
	
	--Read in a string and check the score for each valid word based on its
	  length and print out the result of total number of user's words and
	  user's earned scores 
	
		
