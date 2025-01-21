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
constexpr int NUM_CELLS_SUDOKU = 81;   //  total number of cells in a sudoku puzzle
constexpr int NUM_SUDOKU_CONSTRAINTS = 4; //    cell constraint, row constraint, column constraint, box constraint
constexpr int SUDOKU_COLUMN_CONSTRAINTS = NUM_CELLS_SUDOKU * NUM_SUDOKU_CONSTRAINTS;   //  There should be 324 column headers in a sudoku exact cover problem
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
    DLX(std::vector<std::vector<int>>& matrix);

    //  specific constructor for reducing sudoku puzzles into exact cover problems
    DLX(std::vector<std::vector<int>>& matrix, bool isSudokuFlag);

    ~DLX();

    // Debugging: Print the DLX structure
    void print();

    void printDLXList();

    //  Debugging: Testing Cover and Uncover functions
    void testCoverUncover(int colID);

    //  This method searches for possible solutions for a given exact cover problem
    void search(int searchDepth);

    //  This method searches for possible solutions for a given sudoku adaption of an exact cover problem
    void solveSudokuCover(int searchDepth);

    //  This method is for adding rows for empty cells
    void addRowConstraint(int i, int j, std::vector<int>& values);

    protected:
    //  this flag marks the problem type as sudoku
    bool isSudoku = false;
    //  this flag is set in case of sudoku; if there are at least two valid solutions, no further solutions are sought
    bool skipOtherSolutions = false;
    //  this node is the starting point of the dlx algorithm
    DLXNode* header;
    //  this vector ensures that column header nodes are accessible at any time
    std::vector<DLXNode*> columnHeaders;
    //  this vector saves the current set of nodes, which represent solution rows
    std::vector<DLXNode*> solutionSet;
    //  this keeps track of how many solutions have been found. In case of sudoku, more than one solution means an underdefined sudoku
    int numValidSolutions;
    //  this is a copy of input matrix to carry out operations within the dlx class
    std::vector<std::vector<int>> inputMatrix;
    //  this pointer is for direct access to the input matrix to overwrite it at the end
    std::vector<std::vector<int>> *ptrOutputMatrix = nullptr;


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

    //  This method calculates column constraints which should be set to 1 for a given clue
    std::vector<int> calculateColumnConstraints(int row, int col, int val);

    //  This method calculates the exact DLX row for the row-column-value combination of a sudoku clue
    int calculateRowPosition(int row, int col, int val);

    //  This method decodes a solution set into a non-binary sudoku matrix
    void decodePartialSolution(int dlxRow);

    //  This method saves the first valid solution into outputMatrix
    void saveOutputMatrix();


};
