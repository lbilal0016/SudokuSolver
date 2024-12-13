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

    //  ... intermediate operations here

    //  After all other operations, return the board
    return _board;
}

void sudokuSolver::getIndexRange(BoardIndexRange &indexRange, int boardOneNinth){
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
}

void sudokuSolver::markOriginals(){
    //  ToDo: Mark original elements
    for(size_t i = 0; i < NUM_ROWS; ++i){
        for(size_t j = 0; j < NUM_COLUMNS; ++j){
            if(_board[i][j] != 0){
                //  set marker bit for elements corresponding original elements on the sudoku board
                _possibleValues[i][j] |= MARKER_BIT;
                //  set the original value as possible value for consistency
                setPossibleValue(i,j,_board[i][j]);
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
    if(_possibleValues[row][column] & MARKER_BIT != 0){
        throw IllegalOperationException("Exception occured: Attempt to change an original entry.");
    }else{ 
        //  set bit number value - 1 (0th bit corresponds to value 1)
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

void sudokuSolver::getIntValueFromBit(int &interprtdIntVal, uint16_t bitPos){
    if(bitPos & ALLBITS == 0){
        //  no bit is set, meaning the value is 0: the current square is empty
        interprtdIntVal = 0;
        throw LogicalErrorOccured("Marker bit or value found bit was set, although no value bits are set!");
    }else{
        interprtdIntVal = 1;    //  bits are 1-based
        while((bitPos & BITPOS_1) == 0){
            //  interpreted int value needs to increase
            bitPos >> 1;    //  shift bit to right to check the next bit
            ++interprtdIntVal;
        }
    }
}

int sudokuSolver::isValueKnown(int row, int column){
    int knownValue = 0;
    if((_possibleValues[row][column] & MARKER_BIT != 0) || 
    (_possibleValues[row][column] & VALUE_FOUND_BIT != 0)){
        uint16_t valueBuf = _possibleValues[row][column] & ALLBITS;
        getIntValueFromBit(knownValue, valueBuf);
    }
    return knownValue;
}

int sudokuSolver::isValueFound(int row, int column){
if((_possibleValues[row][column] & MARKER_BIT != 0) ||
    (_possibleValues[row][column] & VALUE_FOUND_BIT != 0))
}

void sudokuSolver::checkOneNinths(){
    BoardIndexRange indexRange;

    for(int i = 1; i <= NUM_NINTHS; ++i){
        // empty eliminated values buffer for current one-ninth
        _bufEliminatedVals = 0; 
        //  get the index range for current one-ninth
        getIndexRange(indexRange, i);

        //  Find to be eliminated values from a one-ninth
        for(int j = indexRange.rowFirst; j <= indexRange.rowLast; ++j){
            for(int k = indexRange.columnFirst; k <= indexRange.columnLast; ++k){
                //  detect a known value from a square
                if(isValueKnown(j,k) != 0){
                    //  set known elements in a uint16_t buffer (base 1)
                    _bufEliminatedVals = _bufEliminatedVals | (1 << (isValueKnown(j,k) - 1));
                }
            }
        }
        //  Eliminate previously determined values from the same one-ninth
        for(int j = indexRange.rowFirst; j <= indexRange.rowLast; ++j){
            for(int k = indexRange.columnFirst; k <= indexRange.columnLast; ++k){
                if(isValueKnown(j,k) == 0){
                    _possibleValues[j][k] &= ~(_bufEliminatedVals);
                    if(isValueFound(j,k) != 0){
                        //  set the bit for found values
                        _possibleValues[j][k] |= VALUE_FOUND_BIT;
                    }
                }
            }
        }
    }
}