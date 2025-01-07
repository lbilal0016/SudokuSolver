#pragma once

#include <cstdint>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <climits>

constexpr int INVALID_ROW = -999; // invalid row index
constexpr int INVALID_COLUMN = -999; // invalid column index
constexpr int COLUMN_HEADER_ROW = -1; // row index value for column headers in DLX data structure
constexpr int NUM_CELLS = 81;   //  total number of cells in a sudoku puzzle
constexpr int NUM_SUDOKU_CONSTRAINTS = 4; //    cell constraint, row constraint, column constraint, box constraint
constexpr int SUDOKU_COLUMN_CONSTRAINTS = NUM_CELLS * NUM_SUDOKU_CONSTRAINTS;   //  There should be 324 column headers in a sudoku exact cover problem
constexpr int SUDOKU_ROWS = 9;
constexpr int SUDOKU_COLUMNS = 9;
constexpr int SUDOKU_NUM_POSSIBILITIES = 9;

constexpr int ROW_CONSTRAINT_OFFSET = 81; //    Row constraints start with column index 81
constexpr int COLUMN_CONSTRAINT_OFFSET = 162;   //  Column constraints start with column index 162
constexpr int BOX_CONSTRAINT_OFFSET = 243;   //  Box constraints start with column index 162

constexpr int ROW_COEFFICIENT_ROW = 81;
constexpr int COLUMN_COEFFICIENT_ROW = 9;
constexpr int SUDOKU_SOLUTION_SPACE = 81;

struct DLXNode{
    DLXNode* left;
    DLXNode* right;
    DLXNode* up;
    DLXNode* down;
    DLXNode* column;

    int rowID;
    int columnID;

    //  null initialization of struct object
    DLXNode(int r = INVALID_ROW, int c = INVALID_COLUMN) : left(this), right(this), up(this), down(this), column(nullptr), rowID(r), columnID(c){}
};

class DLX{
    public:
    //  generic constructor for exact cover problems
    DLX(const std::vector<std::vector<int>>& matrix);

    //  specific constructor for reducing sudoku puzzles into exact cover problems
    DLX(const std::vector<std::vector<int>>& puzzle, bool isSudokuFlag);

    // Debugging: Print the DLX structure
    void print();

    //  Debugging: Testing Cover and Uncover functions
    void testCoverUncover(int colID);

    //  This method searches for possible solutions for a given exact cover problem
    void search(int searchDepth);

    //  This method searches for possible solutions for a given sudoku adaption of an exact cover problem
    void solveSudokuCover(int searchDepth);

    //  This method is for adding rows for empty cells
    void addRowConstraint(int i, int j, std::vector<int>& values);

    protected:
    bool isSudoku = false;
    DLXNode* header;
    std::vector<DLXNode*> columnHeaders;
    std::vector<DLXNode*> solutionSet;
    int numValidSolutions;
    std::vector<std::vector<int>> inputMatrix;

    //  this method adds a new row to the DLX structure
    void addRow(int rowID, const std::vector<int>& columns);

    //  this method suppresses a column during the search for an exact cover problem solution
    void coverColumn(DLXNode* column);

    //  this method reverses the coverColumn method; it makes a previously suppressed node visible again
    void uncoverColumn(DLXNode* column);

    //  This method prints a possible solution set
    void printSolution();

    //  This method chooses the next possible column for a solution set
    DLXNode* chooseColumn();

    //  TODO: This method calculates column constraints which should be set to 1 for a given clue
    std::vector<int> calculateColumnConstraints(int row, int col, int val);

    //  TODO: This method calculates the exact DLX row for the row-column-value combination of a sudoku clue
    int calculateRowPosition(int row, int col, int val);
};
