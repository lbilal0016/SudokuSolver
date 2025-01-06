#include <iostream>

#include "sudokuSolver.h"
#include "DoublyLinkedList.h"

int main(){
    // Example input matrix
    std::vector<std::vector<int>> matrix = {
        {0, 0, 1, 0, 1, 1, 0},
        {1, 0, 0, 1, 0, 0, 1},
        {0, 1, 1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 1},
        {0, 0, 0, 1, 1, 0, 1}
    };

    // Create DLX instance
    DLX dlx(matrix);

    //  test the cover and uncover functions
    //dlx.testCoverUncover(0);

    // Print the structure
    /*
    std::cout << "\nPrinting the original DLX Structure..." << std::endl;
    std::cout << "=========================================" << std::endl;
    dlx.print();
    */
   
    dlx.search(0);

    return 0;
}