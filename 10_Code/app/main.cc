#include <iostream>

#include "sudokuSolver.h"
#include "DoublyLinkedList.h"

int main(){
    // Example input matrix
    std::vector<std::vector<int>> matrix = {
        {1, 0, 0, 1, 0, 0},
        {0, 1, 0, 1, 0, 0},
        {0, 0, 1, 0, 1, 1},
        {1, 0, 0, 0, 1, 0},
        {0, 1, 1, 0, 0, 1}
    };

    // Create DLX instance
    DLX dlx(matrix);

    // Print the structure
    dlx.print();

    return 0;
}