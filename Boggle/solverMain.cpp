#include <iostream>

#include "BogWordList.h"
#include "Dictionary.h"
#include "BogSolver.h"

int main()
{
        BogSolver solver;

        solver.readDict();
        solver.readBoard();
        solver.solve();
        solver.printWords();

        return 0;
}
