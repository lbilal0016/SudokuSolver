#include <iostream>
#include <vector>
#include <string>

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
    if(inputArg.size() != NUM_CELLS){
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
        }

        //  add character to parsedInput vector using ASCII code
        parsedInput.push_back(ch - '0');
    }


 std::cout << "Input argument vector:\n======================\n";
for(int i = 0; i < parsedInput.size(); ++i){
    if(i % 9 == 0) std::cout << "{ "; // Line beginning
    std::cout << parsedInput[i];
    if(i % 9 != 8) std::cout << ", "; // Adding comma
    else std::cout << " }"; // Line end
    if(i % 9 == 8) std::cout << "\n"; // Newline
}


    return 0;
}
