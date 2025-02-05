#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <chrono>

#include "sudokuSolver.h"
#include "DoublyLinkedList.h"

int main(int argc, char* argv[]){

    //  starting timer
    auto timerStart = std::chrono::high_resolution_clock::now();

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

    //  create a dlx solver object with sudoku flag
    DLX dlxSolver(parsedInput, true);

    std::cout << "Sudoku solver started.\n";

    // call sudoku solver of dlx class
    dlxSolver.solveSudokuCover(0);

    // After dlx returns its final result, print a last line
    std::cout << "Sudoku solver terminated with above given results.\nExiting the program ..." << std::endl;

    //  stopping timer
    auto timerEnd = std::chrono::high_resolution_clock::now();

    //  getting elapsed time
    auto timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(timerEnd - timerStart);

    //  printing elapsed time
    std::cout << "Time elapsed from program start to printing results : " << timeElapsed.count() << " ms.\n";

    // Delete dlxSolver along with all the nodes within
    dlxSolver.~DLX();

    return 0;
}
