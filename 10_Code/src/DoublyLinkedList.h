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
    DLX(const std::vector<std::vector<int>>& matrix);

    // Debugging: Print the DLX structure
    void print();

    //  Debugging: Testing Cover and Uncover functions
    void testCoverUncover(int colID);

    //  This method searches for possible solutions for a given exact cover problem
    void search(int searchDepth);

    private:
    DLXNode* header;
    std::vector<DLXNode*> columnHeaders;
    std::vector<DLXNode*> solutionSet;
    int numValidSolutions;

    //  this method adds a new row to the DLX structure
    void addRow(int rowID, const std::vector<int>& columns);

    //  this method suppresses a column during the search for an exact cover problem solution
    void coverColumn(DLXNode* column);

    //  this method reverses the coverColumn method; it makes a previously suppressed node visible again
    void uncoverColumn(DLXNode* column);

    //  TODO: this method prints a possible solution set
    void printSolution();

    //  TODO: this method chooses the next possible column for a solution set
    DLXNode* chooseColumn();
};