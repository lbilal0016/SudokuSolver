#include <iostream>
#include "sudokuSolver.h"


sudokuSolver::sudokuSolver(BoardType &board, bool isSolved) : _board(board), _isSolved(isSolved) {
    //  ToDo: Some False argument check
    
    _originalsMarked = false;

    _markedElements.resize(NUM_ROWS, std::vector<bool>(NUM_COLUMNS, false));    //  shaping NUM_ROWS x NUM_COLUMNS marked elements vector
    _possibleValues.resize(NUM_ROWS, std::vector<std::vector<int>>(NUM_COLUMNS, std::vector<int>(NUM_POSSIBILITIES, 0)));   //  shaping NUM_ROWS x NUM_COLUMNS x NUM_POSSIBILITIES possible values vector
    //  shaping the marked elements vector
    for(size_t i = 0;  i < NUM_ROWS; ++i){
        for(size_t j = 0; j < NUM_COLUMNS; ++j){
            _markedElements[i][j] = false;
        }
    }

    markOriginals();
}

sudokuSolver::sudokuSolver(){
    //  Shape the board and board marker vectors
    for(size_t i = 0;  i < NUM_ROWS; ++i){
        for(size_t j = 0; j < NUM_COLUMNS; ++j){
            _board[i][j] = 0; 
            _markedElements[i][j] = false;
        }
    }
}

BoardType sudokuSolver::solvePuzzle(BoardType &BoardType){
    //  ToDo:   Create a validity check against BoardType

    if(_originalsMarked != true){
    //  Marking down the original elements which are parts of the puzzle and not the solution
    markOriginals();
    }

}

void sudokuSolver::markOriginals(){
    //  ToDo: Mark original elements
    for(size_t i = 0; i < NUM_ROWS; ++i){
        for(size_t j = 0; j < NUM_COLUMNS; ++j){

        }
    }
    //  Finish: set the flag
    _originalsMarked = true;
}