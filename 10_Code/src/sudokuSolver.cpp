#include <iostream>
#include "sudokuSolver.h"


sudokuSolver::sudokuSolver(BoardType &board, bool isSolved) : _board(board), _isSolved(isSolved) {
    //  ToDo: Some False argument check
    _originalsMarked = false;
    _completedColumns = 0;
    _completedRows = 0;
    _completedOneNinths = 0;

    _possibleValues.resize(NUM_ROWS, std::vector<uint16_t>(NUM_COLUMNS, 0));   //  shaping NUM_ROWS x NUM_COLUMNS elements possbility vector
    _indexVecUnknownVals.resize(NUM_POSSIBILITIES, {0,0});
    markOriginals();
}

sudokuSolver::sudokuSolver(std::vector<int> &unformattedBoard, bool isSolved){
    //  ToDo: Some False argument check
    _originalsMarked = false;
    _completedColumns = 0;
    _completedRows = 0;
    _completedOneNinths = 0;

    _board = convertSudokuFormat(unformattedBoard);
    _isSolved = isSolved;

    _possibleValues.resize(NUM_ROWS, std::vector<uint16_t>(NUM_COLUMNS, 0));   //  shaping NUM_ROWS x NUM_COLUMNS elements possbility vector
    _indexVecUnknownVals.resize(NUM_POSSIBILITIES, {0,0});
    markOriginals();
}

BoardType sudokuSolver::solvePuzzle(){
    //  ToDo:   Create a validity check against BoardType

    if(_originalsMarked != true){
    //  Marking down the original elements which are parts of the puzzle and not the solution
    markOriginals();
    }

    //  create a dlxSolver object with sudoku flag
    DLX dlxSolver(_board, true);
    //  Firstly, check rows
    checkRows();
    //  Secondly, check columns
    checkColumns();
    //  Finally, check subgrids (aka oneNinths)
    checkOneNinths();

    /*  after a first evaluation of sudoku with given clues, some values are already eliminated. 
        with what is left, dlxSolver algorithm is updated   */
    for(int row = 0; row < NUM_ROWS; ++row){
        for(int column = 0; column < NUM_COLUMNS; ++column){
            //  add new row constraint if the value in the current cell is not known
            if(isValueKnown(row, column) == 0){
                std::vector<int> remainingPossibleValues = getPossibleValsFromBits(row, column, _possibleValues[row][column]);
                dlxSolver.addRowConstraint(row, column, remainingPossibleValues);
            }
        }
    }

    //  After applying the conditions of eliminated values to dlx algorithm, search function can be called
    dlxSolver.solveSudokuCover(0);

    //  After dlx returns its final result, print a last line
    std::cout << "Sudoku solver terminated with above given results.\nExiting the program ..." << std::endl;

    //  Delete dlxSolver along with all the nodes within
    dlxSolver.~DLX();

    //  After all other operations, return the board
    return _board;
}

bool sudokuSolver::checkPuzzle(){
    for(int row = 0; row < SUDOKU_ROWS; ++row){
        for(int column = 0; column < SUDOKU_COLUMNS; ++column){
            if(_board[row][column] == 0){
                //  there is at least one cell with an unknown value
                return false;
            }
        }
    }
    //  all cells has valid values, puzzle is complete
    return true;
}

BoardType sudokuSolver::convertSudokuFormat(std::vector<int> unformattedSudoku){
    BoardType sudokuBoard;

    //  error handling: see if the input has a right number of numbers for a sudoku puzzle
    if(unformattedSudoku.size() != NUM_CELLS){
        std::cerr << "Size of the input vector given to sudokuSolver does not match a standard sudoku puzzle.\n"
        << "Program terminated ...";
    }

    //  parse a 1D vector containing 81 cells into a sudokuBoard type object
    for(int i = 0; i < unformattedSudoku.size(); ++i){
        int row = i / NUM_ROWS;
        int column = i % NUM_COLUMNS;
        sudokuBoard[row][column] = unformattedSudoku[i];
    }

    return sudokuBoard;
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

std::vector<int> sudokuSolver::getPossibleValsFromBits(int row, int column, uint16_t bitPos){
    std::vector<int> possibleCellValues;
    if(bitPos & ALLBITS == 0){
         throw LogicalErrorOccured("No bit position set for cell\n");
    }

    for(int i = 0; i < NUM_POSSIBILITIES; ++i){
        //  if the current 0th position of bitPos is non-zero, bit position representing (i+1) value is set 
        if((bitPos & (1 << i)) != 0){
            possibleCellValues.push_back(i + 1);    //  bits are 1-based
        }
    }

    return possibleCellValues;
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
    int bufKnownValCounter;

    for(int i = 1; i <= NUM_NINTHS; ++i){
        if((_completedOneNinths & (1 << (i - 1))) != 0){
            //  skip one-ninth if it is completed
            continue;
        }
        // empty eliminated values buffer for current one-ninth
        _bufEliminatedVals = 0; 
        //  resets the index for unknown elements vector 
        _nextElementUnknownValsVec = 0;
        //  resets the counter for known values
        bufKnownValCounter = 0;
        //  get the index range for current one-ninth
        getIndexRange(indexRange, i);

        //  Find to be eliminated values from a one-ninth
        for(int j = indexRange.rowFirst; j <= indexRange.rowLast; ++j){
            for(int k = indexRange.columnFirst; k <= indexRange.columnLast; ++k){
                //  detect a known value from a square
                if(isValueKnown(j,k) != 0){
                    //  set known elements in a uint16_t buffer (base 1)
                    _bufEliminatedVals = _bufEliminatedVals | (1 << (isValueKnown(j,k) - 1));
                    if((++bufKnownValCounter) == NUM_POSSIBILITIES){
                        //  Increase bufKnownValCounter and if it is equal to 9, mark the one-ninth as full
                        _completedOneNinths |= (1 << (i - 1));
                    }
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
                if((++bufKnownValCounter) == NUM_POSSIBILITIES){
                    //  Increase bufKnownValCounter and if it is equal to 9, mark the one-ninth as full
                    _completedOneNinths |= (1 << (i - 1));
                }                
            }
        } 
    }
}

void sudokuSolver::checkRows(){
    int bufKnownValCounter;
    for(int i = 0; i < NUM_ROWS; ++i){
        if((_completedRows & (1 << (i - 1))) != 0){
            //  skip one-ninth if it is completed
            continue;
        }
        //  reset bit buffer for eliminated values
        _bufEliminatedVals = 0;
        //  resets the index for unknown elements vector 
        _nextElementUnknownValsVec = 0;
        //  resets the counter for known values
        bufKnownValCounter = 0;

        //  Find to be eliminated values for ith row
        for(int j = 0; j < NUM_COLUMNS; ++j){
            if(isValueKnown(i,j) != 0){
                //  set known elements in a uint16_t buffer (base 1)
                _bufEliminatedVals = _bufEliminatedVals | (1 << (isValueKnown(i,j) - 1));
                if((++bufKnownValCounter) == NUM_POSSIBILITIES){
                    //  Increase bufKnownValCounter and if it is equal to 9, mark the row as full
                    _completedRows |= (1 << (i - 1));
                }
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
                if((++bufKnownValCounter) == NUM_POSSIBILITIES){
                    //  Increase bufKnownValCounter and if it is equal to 9, mark the row as full
                    _completedRows |= (1 << (i - 1));                    
                }
            }
        }
    }
}

void sudokuSolver::checkColumns(){
    //  i and j are intentionally swapped for vector convention 
    //  i represents rows, while j represents columns
    int bufKnownValCounter;

    for(int j = 0; j < NUM_COLUMNS; ++j){
        if((_completedColumns & (1 << (j - 1))) != 0){
            //  skip one-ninth if it is completed
            continue;
        }
        //  reset bit buffer for eliminated values
        _bufEliminatedVals = 0;
        //  resets the index for unknown elements vector 
        _nextElementUnknownValsVec = 0;
        //  resets the counter for known values
        bufKnownValCounter = 0;

        //  Find to be eliminated values for jth column
        for(int i = 0; i < NUM_ROWS; ++i){
            if(isValueKnown(i,j) != 0){
                //  set known elements in a uint16_t buffer (base 1)
                _bufEliminatedVals = _bufEliminatedVals | (1 << (isValueKnown(i,j) - 1));
                if((++bufKnownValCounter) == NUM_POSSIBILITIES){
                    //  Increase bufKnownValCounter and if it is equal to 9, mark the column as full
                    _completedColumns |= (1 << (j - 1));
                }
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
                if((++bufKnownValCounter) == NUM_POSSIBILITIES){
                    //  Increase bufKnownValCounter and if it is equal to 9, mark the column as full
                    _completedColumns |= (1 << (j - 1));
                }
            }
        }
    }
}