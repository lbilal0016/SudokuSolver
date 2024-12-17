#include <iostream>
#include "sudokuSolver.h"


sudokuSolver::sudokuSolver(BoardType &board, bool isSolved) : _board(board), _isSolved(isSolved) {
    //  ToDo: Some False argument check
    
    _originalsMarked = false;

    _possibleValues.resize(NUM_ROWS, std::vector<uint16_t>(NUM_COLUMNS, 0));   //  shaping NUM_ROWS x NUM_COLUMNS elements possbility vector
    _indexVecUnknownVals.resize(NUM_POSSIBILITIES, {0,0});
    markOriginals();
}

sudokuSolver::sudokuSolver(){
    _originalsMarked = false;
    _isSolved = false;

    //  shaping member vectors
    _board.resize(NUM_ROWS, std::vector<int>(NUM_COLUMNS, 0));  //  shaping NUM_ROWS x NUM_COLUMNS board vector
    _possibleValues.resize(NUM_ROWS, std::vector<uint16_t>(NUM_COLUMNS, 0));   //  shaping NUM_ROWS x NUM_COLUMNS elements possbility vector
    _indexVecUnknownVals.resize(NUM_POSSIBILITIES, {0,0});
}

BoardType sudokuSolver::solvePuzzle(BoardType &Board){
    //  ToDo:   Create a validity check against BoardType

    if(_originalsMarked != true){
    //  Marking down the original elements which are parts of the puzzle and not the solution
    markOriginals();
    }

    do
    {
        //  First iteration should run without completeness check
        //  all other iterations are meant, if the puzzle is not yet complete
        
        //  Firstly, check rows
        checkRows();
        //  Secondly, check columns
        checkColumns();
        //  Finally, check subgrids (aka oneNinths)
        checkOneNinths();
    } while (!checkPuzzle());

    //  After all other operations, return the board
    return _board;
}

bool sudokuSolver::checkPuzzle(){
    //  ToDo: Use the same logic for check rows, columns and one-ninths to determine
    //  whether the puzzle is complete
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

bool sudokuSolver::isFound(int row, int column){
    //  If found bit is set, the value is found
    return _possibleValues[row][column] & VALUE_FOUND_BIT != 0;
}

void sudokuSolver::setPossibleValue(int row, int column, int value){
    if(isOriginal(row,column)){
        throw IllegalOperationException("Exception occured: Attempt to change an original entry.");
    }else{ 
        //  set bit number value - 1 (0th bit corresponds to value 1)
        _possibleValues[row][column] = _possibleValues[row][column] | (1 << (value - 1));
    }
}

void sudokuSolver::removeImpossibleValue(int row, int column, int value){
    //  reset bit number value - 1 (0th bit corresponds to value 1)
    if(isOriginal(row,column)){
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
    if(isOriginal(row,column) || isFound(row,column)){
        uint16_t valueBuf = _possibleValues[row][column] & ALLBITS;
        getIntValueFromBit(knownValue, valueBuf);
    }
    return knownValue;
}

int sudokuSolver::isValueFound(int row, int column){
    if(isOriginal(row,column) || isFound(row,column)){
        //  Function called although value of the square is known
            throw LogicalErrorOccured("Logical error: isValueKnown function called while value of a square is known");        
    }

    //  found value will be saved in this variable
    int foundValue = 0;
    //  this counter keeps track of how many squares are marked with 1 bit
    int setBitsCounter = 0;
    for(int i = 1; i <= NUM_POSSIBILITIES; ++i){
        if(_possibleValues[row][column] & (1 << (i - 1)) != 0){
            //  increase the setBitsCounter as soon as a 1 bit is found
            ++setBitsCounter;
            if(setBitsCounter < 2){ 
                //  only one 1 bit was found so far
                foundValue = i;
            }else{
                //  more than one 1 bit was found. Value not yet known, no need to iterate further
                break;
            }
        }
    }

    if(setBitsCounter == 0){
        // All values were eliminated somehow, there is a logical error
        throw LogicalErrorOccured("Logical error: All values eliminated for a square");
    }else if(setBitsCounter == 1){
        //  error check
        if(_possibleValues[row][column] & (1 << (foundValue - 1)) == 0){
            throw LogicalErrorOccured("Logical error: Found value does not match the bit set");
        }else if(_board[row][column] != 0){
            throw IllegalOperationException("Illegal operation: Attempt to change a known board value");
        }else{
            //  enter the found value in main board
            _board[row][column] = foundValue;
        }
    }else if(setBitsCounter > 1){
        //  There are still more than one possible value, zero is returned
        foundValue = 0;
    }
    
    return foundValue;
}

void sudokuSolver::checkOneNinths(){
    BoardIndexRange indexRange;

    for(int i = 1; i <= NUM_NINTHS; ++i){
        // empty eliminated values buffer for current one-ninth
        _bufEliminatedVals = 0; 
        //  resets the index for unknown elements vector 
        _nextElementUnknownValsVec = 0;
        //  get the index range for current one-ninth
        getIndexRange(indexRange, i);

        //  Find to be eliminated values from a one-ninth
        for(int j = indexRange.rowFirst; j <= indexRange.rowLast; ++j){
            for(int k = indexRange.columnFirst; k <= indexRange.columnLast; ++k){
                //  detect a known value from a square
                if(isValueKnown(j,k) != 0){
                    //  set known elements in a uint16_t buffer (base 1)
                    _bufEliminatedVals = _bufEliminatedVals | (1 << (isValueKnown(j,k) - 1));
                }else{
                    //  save squares whose values are currently not known
                    _indexVecUnknownVals[_nextElementUnknownValsVec++] = {j,k};
                }
            }
        }

        //  Eliminate previously determined values from unknown squares of the same one-ninth
        for(int i = 0; i < _nextElementUnknownValsVec; ++i){
            int row = _indexVecUnknownVals[i].row;
            int column = _indexVecUnknownVals[i].column;
            _possibleValues[row][column] &= ~(_bufEliminatedVals);
            if(isValueFound(row, column) !=  0){
                //  set the bit for found values
                _possibleValues[row][column] |= VALUE_FOUND_BIT;
            }
        } 
    }
}

void sudokuSolver::checkRows(){
    for(int i = 0; i < NUM_ROWS; ++i){
        //  reset bit buffer for eliminated values
        _bufEliminatedVals = 0;
        //  resets the index for unknown elements vector 
        _nextElementUnknownValsVec = 0;
        //  Find to be eliminated values for ith row
        for(int j = 0; j < NUM_COLUMNS; ++j){
            if(isValueKnown(i,j) != 0){
                //  set known elements in a uint16_t buffer (base 1)
                _bufEliminatedVals = _bufEliminatedVals | (1 << (isValueKnown(i,j) - 1));
            }else{
                //  save squares whose values are currently not known
                _indexVecUnknownVals[_nextElementUnknownValsVec++] = {i,j};
            }
        }

        //  Eliminate previously determined values from empty elements of ith row
        for(int i = 0; i < _nextElementUnknownValsVec; ++i){
            int row = _indexVecUnknownVals[i].row;
            int column = _indexVecUnknownVals[i].column;
            _possibleValues[row][column] &= ~(_bufEliminatedVals);
            if(isValueFound(row,column) != 0){
                // set the bit for found values
                _possibleValues[row][column] |= VALUE_FOUND_BIT;
            }
        }
    }
}

void sudokuSolver::checkColumns(){
    //  i and j are intentionally swapped for vector convention 
    //  i represents rows, while j represents columns
    for(int j = 0; j < NUM_COLUMNS; ++j){
        //  reset bit buffer for eliminated values
        _bufEliminatedVals = 0;
        //  resets the index for unknown elements vector 
        _nextElementUnknownValsVec = 0;
        //  Find to be eliminated values for jth column
        for(int i = 0; i < NUM_ROWS; ++i){
            if(isValueKnown(i,j) != 0){
                //  set known elements in a uint16_t buffer (base 1)
                _bufEliminatedVals = _bufEliminatedVals | (1 << (isValueKnown(i,j) - 1));
            }else{
                _indexVecUnknownVals[_nextElementUnknownValsVec++] = {i,j};
            }
        }

        //  Eliminate previously determined values from empty elements of jth column
        for(int i = 0; i < _nextElementUnknownValsVec; ++i){
            int row = _indexVecUnknownVals[i].row;
            int column = _indexVecUnknownVals[i].column;
            _possibleValues[row][column] &= ~(_bufEliminatedVals);
            if(isValueFound(row,column) != 0){
                _possibleValues[row][column] |= VALUE_FOUND_BIT;
            }
        }
    }
}