#pragma once
#include <cstdint>
#include <vector>

#define NUM_ROWS 9
#define NUM_COLUMNS 9
#define NUM_POSSIBILITIES 9

using BoardType = std::vector<std::vector<int>>;
using BoardMarker = std::vector<std::vector<bool>>;

struct BoardIndex
{
    int i;  //  first dimension of BoardType vector
    int j;  //  second dimension of BoardType vector
};


class sudokuSolver{
    public:
        sudokuSolver(BoardType &board, bool isSolved);
        sudokuSolver();

        ~sudokuSolver();

        //  This function is for solving a non-finished sudoku
        BoardType solvePuzzle(BoardType &BoardType);

        //  This function is for completed sudokus, whose correctness is checked
        bool checkPuzzle();

    private:
        BoardType _board;
        std::vector<BoardType> _possibleValues;
        BoardMarker _markedElements;

        bool _isSolved;
        bool _originalsMarked;
        

        //  This function returns vector index of a target element whose row and column is known
        BoardIndex getElementIndex(int targetRow, int targetColumn);

        //  This function marks elements from sudoku itself, such that they cannot be changed
        void markOriginals();

        //  This function checks whether an element is an original element or a solved element
        bool isOriginal();
};