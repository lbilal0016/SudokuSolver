#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

#include "sudokuSolver.h"
#include "DoublyLinkedList.h"

int main(int argc, char* argv[]){
    //  input argument error handling
    if(argc < 2){
        std::cerr << "Input argument usage wrong.\n"
        <<  "Correct usage: sudokuSolver.exe 530...000" << std::endl;
        return 1;
    }


    std::string inputArg = argv[1];
    if(inputArg.size() != NUM_CELLS_SUDOKU){
        std::cerr << "Program argument should contain 81 numbers for each sudoku cell.\n";
        return 1;
    }

    std::vector<int> parsedInput;
    for(char ch : inputArg){
        //  Error handling for false character input
        if(ch < '0' || ch > '9'){
            std::cerr << "There is an invalid character in input argument: "
            << ch << std::endl
            << "Program aborted...\n";
            return 1;
        }

        //  add character to parsedInput vector using ASCII code
        parsedInput.push_back(ch - '0');
    }

    //  create a sudoku solver object with unsolved flag
    sudokuSolver solver(parsedInput, false);

    std::cout << "Sudoku solver started.\n";

    BoardType boardTypeInput = solver.convertSudokuFormat(parsedInput);
    std::cout << "Sudoku input was converted to sudoku board format.\n";

    //  let the solver handle the sudoku puzzle
    //  solver.solvePuzzle();

    //  create a dlxSolver object with sudoku flag
    DLX dlxSolver(boardTypeInput, true);

    //  After applying the conditions of eliminated values to dlx algorithm, search function can be called
    dlxSolver.solveSudokuCover(0);

    //  After dlx returns its final result, print a last line
    std::cout << "Sudoku solver terminated with above given results.\nExiting the program ..." << std::endl;

    //  Delete dlxSolver along with all the nodes within
    dlxSolver.~DLX();

    //  destroy sudokuSolver object
    solver.~sudokuSolver();

    return 0;
}
