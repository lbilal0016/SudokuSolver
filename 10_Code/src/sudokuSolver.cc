#include <iostream>
#include "sudokuSolver.h"


sudokuSolver::sudokuSolver(BoardType &board, bool isSolved) : _board(board), _isSolved(isSolved) {
    //  ToDo: Some False argument check
    
    _originalsMarked = false;

    _possibleValues.resize(NUM_ROWS, std::vector<uint16_t>(NUM_COLUMNS, 0));   //  shaping NUM_ROWS x NUM_COLUMNS elements possbility vector

    markOriginals();
}

sudokuSolver::sudokuSolver(){
    _originalsMarked = false;
    _isSolved = false;

    //  shaping member vectors
    _board.resize(NUM_ROWS, std::vector<int>(NUM_COLUMNS, 0));  //  shaping NUM_ROWS x NUM_COLUMNS board vector
    _possibleValues.resize(NUM_ROWS, std::vector<uint16_t>(NUM_COLUMNS, 0));   //  shaping NUM_ROWS x NUM_COLUMNS elements possbility vector
}

BoardType sudokuSolver::solvePuzzle(BoardType &Board){
    //  ToDo:   Create a validity check against BoardType

    if(_originalsMarked != true){
    //  Marking down the original elements which are parts of the puzzle and not the solution
    markOriginals();
    }

    //  After all other operations, return the board
    return _board;
}

BoardIndexRange sudokuSolver::getIndexRange(int boardOneNinth){
    BoardIndexRange indexRange;
    if(!(0 < boardOneNinth < 10)){
        throw std::invalid_argument("Please give a board ninth between [1-9]");
    }

    //  determine row range
    if(boardOneNinth <= 3){
        indexRange.rowFirst = 0;
        indexRange.rowLast = 2;
    }else if(boardOneNinth <= 6){
        indexRange.rowFirst = 3;
        indexRange.rowLast = 5;
    }else{
        indexRange.rowFirst = 6;
        indexRange.rowLast = 8;
    }

    //  determine column range
    if(boardOneNinth == 1 || boardOneNinth == 4 || boardOneNinth == 7){
        indexRange.columnFirst = 0;
        indexRange.columnLast = 2;
    }else if(boardOneNinth == 2 || boardOneNinth == 5 || boardOneNinth == 8){
        indexRange.columnFirst = 3;
        indexRange.columnLast = 5;
    }else{
        indexRange.columnFirst = 6;
        indexRange.columnLast = 8;
    }

    return indexRange;
}

void sudokuSolver::markOriginals(){
    //  ToDo: Mark original elements
    for(size_t i = 0; i < NUM_ROWS; ++i){
        for(size_t j = 0; j < NUM_COLUMNS; ++j){
            if(_board[i][j] != 0){
                //  set market bit for elements corresponding original elements on the sudoku board
                _possibleValues[i][j] |= MARKER_BIT;
            }else{
                //  set all possible bits for elements not corresponding original elements on the sudoku board
                _possibleValues[i][j] |= ALLBITS;
            }
        }
    }
    //  Finish: set the flag
    _originalsMarked = true;
}

bool sudokuSolver::isOriginal(int row, int column){
    //  If marker bit is set, the value is original
    return _possibleValues[row][column] & MARKER_BIT != 0;
}

void sudokuSolver::setPossibleValue(int row, int column, int value){
    //  set bit number value - 1 (0th bit corresponds to value 1)
    if(_possibleValues[row][column] & MARKER_BIT != 0){
        throw IllegalOperationException("Exception occured: Attempt to change an original entry.");
    }else{ 
        _possibleValues[row][column] = _possibleValues[row][column] | (1 << (value - 1));
    }
}

void sudokuSolver::removeImpossibleValue(int row, int column, int value){
    //  reset bit number value - 1 (0th bit corresponds to value 1)
    if(_possibleValues[row][column] & MARKER_BIT != 0){
        throw IllegalOperationException("Exception occured: Attempt to change an original entry.");
    }else{ 
        _possibleValues[row][column] = _possibleValues[row][column] & ~(1 << (value - 1));
    }
}

void sudokuSolver::checkOneNinths(){
    BoardIndexRange indexRange;
    for(int i = 1; i <= NUM_NINTHS; ++i){
        indexRange = getIndexRange(i);
        for(int j = indexRange.rowFirst; j <= indexRange.rowLast; ++j){
            for(int k = indexRange.columnFirst; k <= indexRange.columnLast; ++k){
                
            }
        }
    }
}