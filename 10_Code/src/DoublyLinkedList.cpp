#include "DoublyLinkedList.h"

DLX::DLX(std::vector<std::vector<int>>& matrix){
    numValidSolutions = 0;
    numColumns = 0;
    int numColumns = matrix[0].size();
    header = new DLXNode();
    ++numColumns;
    columnHeaders.resize(numColumns);
    logFile.open(projectPath + "\\log.txt", std::ios::out | std::ios::trunc);
    std::cout << "Log file is saving at : " << projectPath << std::endl;

    //  create column headers
    DLXNode* prev = header;
    for(int i = 0; i < numColumns; ++i){
        DLXNode* col = new DLXNode(COLUMN_HEADER_ROW,i);
        ++numColumns;
        columnHeaders[i] = col;
        prev->right = col;
        prev->column = prev;
        col->left = prev;
        prev = col;
    }
    prev->right = header;
    header->left = prev;

    //  Adding rows from the input matrix
    for(int i = 0; i < matrix.size(); ++i){
        std::vector<int> columns;
        //  Getting the id information from non-empty columns from the current row
        for(int j = 0; j < matrix[i].size(); ++j){
            if(matrix[i][j] != 0){
                columns.push_back(j);
            }
        }

        //  adding a new row with elements on non-empty column slots
        if(!columns.empty()){
            addRow(i, columns);
        }
    }
}

DLX::DLX(std::vector<std::vector<int>>& matrix, bool isSudokuFlag){
    //  safety mechanism: false function call
    if(!isSudokuFlag){
        std::cerr << "Faulty function call, if you wish to solve a sudoku puzzle, second argument should be true\n"
        << "Exiting program ...\n";
    }
    isSudoku = isSudokuFlag;
    numColumns = 0;
    logFile.open(projectPath + "\\log.txt", std::ios::out | std::ios::trunc);
    std::cout << "Log file is saving at : " << projectPath << std::endl;

    logFile << "Debugging line: DLX constructor with sudoku flag.\n";
    logFile.flush();

    //  safety mechanism: wrong matrix size
    if(matrix.size() != SUDOKU_ROWS || matrix[0].size() != SUDOKU_COLUMNS){
        std::cerr << "Provided input matrix does not correspond to a standard sudoku grid.\n"
        << "Exiting program ...\n";
    }

    /*  preformat solutionSet object to optimize memory allocation time (only valid for sudoku problems
    which always have a fixed solution space)   */
    solutionSet.resize(SUDOKU_SOLUTION_SPACE);

    //  create the starting point for DLX structure
    header = new DLXNode();
    ++numColumns;
    //  in the case of a (standard) sudoku, number of columns (or number of constraints) is predefined
    columnHeaders.resize(SUDOKU_COLUMN_CONSTRAINTS);
    //  save a copy of input matrix here
    inputMatrix = matrix;
    //  save the input matrix itself here
    ptrOutputMatrix = &matrix;

    //  create column headers
    DLXNode* prev = header;
    for(int i = 0; i < SUDOKU_COLUMN_CONSTRAINTS; ++i){
        DLXNode* col = new DLXNode(COLUMN_HEADER_ROW,i);
        ++numColumns;
        columnHeaders[i] = col;
        prev->right = col;
        prev->column = prev;
        col->left = prev;
        prev = col;
    }
    prev->right = header;
    header->left = prev;

    /*  DEBUGGING LINE : START */
    logFile << "\tNumber of columns after creating column headers = " << numColumns << std::endl;
    logFile.flush();
    /*  DEBUGGING LINE : END */

    //  add dlx rows based on non-zero sudoku elements
    for(int i = 0; i < matrix.size(); ++i){
        for(int j = 0; j < matrix[i].size(); ++j){
            //  there is no clue in this coordinate, add all possible values
            if(matrix[i][j] == 0){
                for(int val = 1; val <= SUDOKU_NUM_POSSIBILITIES; ++val){
                    logFile << "\tUnknown cell: [" << i << "][" << j <<"] | Possible value: "
                    << val << "\n\t";
                    addRow(calculateRowPosition(i, j, val), calculateColumnConstraints(i, j, val));
                }
            }
            //  error: clue is invalid for a standard sudoku problem
            else if(matrix[i][j] < 0 || matrix[i][j] > 9){
            
                std::cerr << "Element[" << i << ", " << j << "] is not a valid number for a sudoku puzzle.\n"
                << "Exiting program ...\n";
            }
            //  there is a clue in the current cell; add only known value for this cell
            else{
                std::vector<int> columnsToBeCoveredConstraints = calculateColumnConstraints(i, j, matrix[i][j]);
                //  calculate the row position of the sudoku clue
                int rowPosition = calculateRowPosition(i, j, matrix[i][j]);

                /*  DEBUGGING LINE : START */
                logFile << "\tClue:\n";
                logFile << "\tSudoku element [" << i << "][" << j << "] = " << matrix[i][j]
                << " | Row position = " << rowPosition << "\n\t";
                logFile.flush();
                /*  DEBUGGING LINE : END */

                //  add corresponding row to exact cover matrix with proper columns are equal to 1
                addRow(rowPosition, columnsToBeCoveredConstraints);
                
                /*  add the first element created by the sudoku puzzle clues directly in the solutionSet, 
                    so that algorithm is forced to include these in the solution   */
                DLXNode* firstRowElementColumnsCovered = columnHeaders[columnsToBeCoveredConstraints[0]]->down;
                //  so many solutions as the sudoku clues are added to the solutionSet up front
                solutionSet.push_back(firstRowElementColumnsCovered);
            }
        }
    }

/*  DEBUGGING LINE : START */
logFile << "Debugging line: DLX constructor header position:\n";
logFile << "\tHeader->left | Column = " << header->left->columnID << ", Row = " << header->left->rowID << std::endl;
logFile << "\tHeader->right | Column = " << header->right->columnID << ", Row = " << header->right->rowID << std::endl;
logFile << "\tHeader->up | Column = " << header->up->columnID << ", Row = " << header->up->rowID << std::endl;
logFile << "\tHeader->down | Column = " << header->down->columnID << ", Row = " << header->down->rowID << std::endl;
logFile << "\tHeader->column | Column = " << header->column->columnID << ", Row = " << header->column->rowID << std::endl;
logFile << "\tHeader = " << header << " | Header->right = " << header->right << " | Header->left = " << header->left << std::endl;
logFile.flush();
/*  DEBUGGING LINE : END */
}

DLX::~DLX(){
    //  In deconstructor, all nodes created for dlx structure must be released in memory
    DLXNode* temp = new DLXNode();
    for(DLXNode* column = header->left; column != header; column = column->left){
        //  delete previous column element. The current column element is still needed
        if(temp->up != temp){
            delete temp->up;
            temp->up = nullptr;
        }

        for(DLXNode* row = column->up; row != column; row = row->up){
            //  delete previous row element. The current row element is still needed.
            if(temp->right != temp){
                delete temp->right;
                temp->right = nullptr;
            }
            temp->right = row;
        }
        //  delete the last row element in the current column
        delete temp->right;
        temp->right = temp;

        //  pass the current column element to temp to be deleted during the next iteration
        temp->up = column;
    }
    //  delete the last column element
    delete temp->up;
    temp->up = nullptr;
    temp->right = nullptr;

    //  delete the temp itself
    delete temp;
    temp = nullptr;

    //  delete the very last dlx element: the header
    delete header;
    header = nullptr;
}

void DLX::addRow(int rowID, const std::vector<int>& columns){

    logFile << "Debugging line: addRow()\n\t";
    logFile.flush();

    DLXNode* first = nullptr;
    for(int colID : columns){
        DLXNode* colHeader = columnHeaders[colID];  //  column header for the newly added element
        DLXNode* newNode = new DLXNode(rowID, colID);   //  coordinate value for the newly added element

        //  link to the column
        newNode->column = colHeader;    //  this value is same for all the elements in the same column
        newNode->down = colHeader;  //  after the lastly added element we return back to column header
        newNode->up = colHeader->up; // previous last value in the current column is the second to last value now
        colHeader->up->down = newNode; //   down pointer to the previous last shows our newly added node now
        colHeader->up = newNode;    //  our newly added node is the last element in the column now


        //  link row horizontally
        if(first != nullptr)
        //  meaning we have already added a first element in the current row
        {
            newNode->left = first->left;    //  previous last element in the row is now second to last
            newNode->right = first;
            first->left->right = newNode;   //  new element is now to the right of previous last
            first->left = newNode;  //  new element is now to the left of the first element
        }
        else
        //  meaning we are only currently adding the very first element to the current row
        {
            first = newNode;
            newNode->right = newNode;
            newNode->left = newNode;
        }
    }

    /*  DEBUGGING LINE : START  */
    logFile << "Row " << first->rowID << " | Columns = ";
    logFile << first->columnID << " ";
    logFile.flush();
    for(DLXNode *rowElement = first->right; rowElement != first; rowElement = rowElement->right){
        logFile << rowElement->columnID << " ";
    }
    logFile << std::endl;
    logFile.flush();
    /*  DEBUGGING LINE : END  */
}

void DLX::print(){
    // Print column headers
    std::cout << "header----";
    for (size_t i = 0; i < columnHeaders.size(); ++i) {
        std::cout << i << "------";
    }
    std::cout << std::endl;

    // For each row, print column links
    for (int rowID = 0; rowID < static_cast<int>(columnHeaders.size()); ++rowID) {
        // Başlangıçta satır numarası kadar boşluk bırak
        std::cout << "Row " << rowID << ": ";

        // Print column links in current row
        for (size_t colID = 0; colID < columnHeaders.size(); ++colID) {
            DLXNode* col = columnHeaders[colID];
            DLXNode* row = col->down;
            bool found = false;

            // Print asterix if there is a link in row
            while (row != col) {
                if (row->rowID == rowID) {
                    found = true;
                    break;
                }
                row = row->down;
            }

            // Print asterix or space
            if (found) {
                std::cout << "   *   ";
            } else {
                std::cout << "       ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void DLX::testCoverUncover(int colID){
    DLXNode* columnHeader = columnHeaders[colID];

    std::cout << "Starting Cover/Uncover method testing ..." << std::endl;
    std::cout << "DLX Structure before covering column " << colID << std::endl;
    print();
    std::cout << std::endl;
    coverColumn(columnHeader);
    std::cout << "DLX Structure after covering column " << colID << std::endl;
    print();
    std::cout << std::endl;
    uncoverColumn(columnHeader);
    std::cout << "DLX Structure after uncovering column " << colID << std::endl;
    print();
}

void DLX::search(int searchDepth){
    //  safety mechanism: seach method is called for a sudoku problem
    if(isSudoku){
        std::cerr << "Faulty class method call: for sudoku problems use solveSudokuCover method.\n"
        << "exiting program ...\n";
    }

    /*  Debugging line:
    std::cout << "Search depth: " << searchDepth << std::endl;
    */

    //  Check if a solution has already been found
    if(header->right == header){
        printSolution();
        //  if all columns were covered successfully, function returns here, indicating a valid solution
        return;
    }

    DLXNode* column = chooseColumn();//header->right;

    for(DLXNode* row = column->down; row != column; row = row->down){
        /*  Debugging line:
        std::cout << "Selected column: " << column->columnID << std::endl;
        */

        //  add this row to the solution set, assuming it is a valid solution
        solutionSet.push_back(row);
        //  cover the column in which solution is assumed to be 
        column->left->right = column->right;
        column->right->left = column->left;
        for(DLXNode* node = row->right; node != row; node = node->right){
            coverColumn(node->column);
        }

        /*  Debugging Line
        std::cout << "Remaining columns: ";
        for(DLXNode* columns = header->right; columns != header; columns = columns->right){
            std::cout << columns->columnID << " ";
        }
        std::cout << std::endl;
        */

        /*  after the matrix is reduced, repeat the steps with a reduced matrix,
            until a search function returns with or without a valid solution
        */
        search(searchDepth + 1);
        //  start from the last solution set element to repeat all steps backwards
        DLXNode* lastSolution = solutionSet.back();
        //  remove the last element from solution set
        solutionSet.pop_back();
        for(DLXNode* node = lastSolution->left; node != lastSolution; node = node->left){
            uncoverColumn(node->column);
        }

        //  uncover the column in which solution has just been sought
        column->left->right = column;
        column->right->left = column;
    }
    /*  if in any depth, a column not has no elements remaining downwards,
    the outer for loop is skipped and search function will return without a valid solution at this point
    */

   /*   Debugging Line:
    std::cout << "Matrix element [" << solutionSet.front()->rowID <<
    ", " << solutionSet.front()->columnID << "] is not a valid solution.\n";
    */

    return;
}

void DLX::solveSudokuCover(int searchDepth){
    logFile << "Debugging line: solveSudokuCover | Depth level = " << searchDepth << ".\n";
    logFile.flush();

    //  Check if a solution has already been found
    if(header->right == header){
        //  current solution is the first one to be found

        /*  DEBUGGING LINE: START   */
        logFile << "Solution found.\n";
        logFile.flush();
        /*  DEBUGGING LINE: END   */

        if(++numValidSolutions == 1){
            //  save the first valid solution in output matrix
            saveOutputMatrix();
            //  then print the possible sudoku solution
            printSolution();
        }
        //  if at least two valid solutions are found, sudoku is underdefined and only one solution will be given out
        else if(numValidSolutions > 1){
            skipOtherSolutions = true;
            printSolution();
        }
        
        /*  DEBUGGING LINE: START   */
        logFile << "Debugging line: solveSudokuCover | header->right == header.\n";
        logFile.flush();
        /*  DEBUGGING LINE: END   */
        return;
    }

    DLXNode* column = chooseColumn();   //  choose a column with minimum number of elements

    logFile << "Debugging line: solveSudokuCover | Main for loop in search function start\n";
    logFile << "\tDebugging line: row = " << column->down << " | column = " << column << std::endl;
    logFile.flush();
    for(DLXNode* row = column->down; row != column; row = row->down){
        //  assuming the node we just hit is a valid partial solution, we add this (temporarily to solutionSet)
        logFile << "Debugging line: solveSudokuCover | Adding an element to solution set ...\n";
        solutionSet.push_back(row);
        logFile << "Debugging line: solveSudokuCover | Adding element to solution set done\n\n";
        logFile.flush();

        //  cover the column in which solution is assumed to be
        column->left->right = column->right;
        column->right->left = column->left;
        --numColumns;

        /*  DEBUGGING LINE : START  */
        logFile << "Adding a partial solution to solution set..." << std::endl
        << "Number of columns : " << (numColumns + 1) << " -> " << numColumns << std::endl;
        logFile.flush();
        /*  DEBUGGING LINE : END  */

        logFile << "Debugging line: solveSudokuCover | covercolumn ...\n";
        logFile.flush();
        for(DLXNode* node = row->right; node != row; node = node->right){
            coverColumn(node->column);
        }
        logFile << "Debugging line: solveSudokuCover | covercolumn done\n\n";

        /*  after the matrix is reduced, repeat the steps with a reduced matrix,
            until a search function returns with or without a valid solution
        */

       logFile << "Debugging line: solveSudokuCover | diving into the next depth ...\n";
       logFile.flush();
       solveSudokuCover(searchDepth + 1);

       //   At least two valid solutions have been found, no more solution iterations
       if(skipOtherSolutions){
            return;
       }

        //  start backtracking from the last partial solution
        DLXNode* lastSolution = solutionSet.back();
        //  remove the last element from solutionSet
        solutionSet.pop_back();
        //  start uncovering from the last partial solution
        for(DLXNode* node = lastSolution->left; node != lastSolution; node = node->left){
            uncoverColumn(node->column);
        }

        //  uncover the column in which solution has just been sought
        column->left->right = column;
        column->right->left = column;        
    }
    logFile << "Debugging line: solveSudokuCover | Main for loop in search function end\n";
    logFile.flush();

    /*  if in any depth, a column not has no elements remaining downwards,
    the outer for loop is skipped and search function will return without a valid solution at this point
    */

    return;
}

void DLX::addRowConstraint(int i, int j, std::vector<int>& values){
    for(int val : values){
        addRow(calculateRowPosition(i, j, val), calculateColumnConstraints(i, j, val));
    }
}

void DLX::coverColumn(DLXNode* column){
    /*  DEBUGGING LINE : START  */
    logFile << "Debugging line: coverColumn()...\n";
    logFile << "\tNumber of available columns = " << numColumns << " -> " << (numColumns - 1) << std::endl;
    logFile.flush();
    /*  DEBUGGING LINE : END  */

    column->left->right = column->right;
    column->right->left = column->left;
    --numColumns;

    

    //  apply this for all rows as long as row is not equal to column
    for(DLXNode* row = column->down; row != column; row = row->down){
        //  suppress all other nodes in current row
        /*  It is important to note that links are only overwritten in vertical direction
            horizontal links remain intact (node->right / node->left) to leave some
            access to the node that is currently being deleted (or rather suppressed)
        */
        for(DLXNode* node = row->right; node != row; node = node->right){
            node->up->down = node->down;
            node->down->up = node->up;
        }
    }

    /*  DEBUGGING LINE : START  */
    logFile << "Debugging line: coverColumn() done\n\n";
    logFile.flush();
    /*  DEBUGGING LINE : END  */
}

void DLX::uncoverColumn(DLXNode* column){
    /*  DEBUGGING LINE : START  */
    logFile << "Debugging line: uncoverColumn()...\n";
    logFile << "\tNumber of available columns = " << (numColumns - 1) << " -> " << numColumns << std::endl;
    logFile.flush();
    /*  DEBUGGING LINE : END  */

    //  reverse the outer for loop in coverColumn method
    for(DLXNode* row = column->up; row != column; row = row->up){
        //  reverse the inner for loop in coverColumn method
        for(DLXNode* node = row->left; node != row; node = node->left){
           node->up->down = node;
           node->down->up = node;
        }
    }

    //  update column header links (make suppressed column header reappear)
    column->left->right = column;
    column->right->left = column;
    ++numColumns;
    
    
    /*  DEBUGGING LINE : START  */
    logFile << "Debugging line: uncoverColumn() done\n\n";
    logFile.flush();
    /*  DEBUGGING LINE : END  */
}

void DLX::printSolution(){
    //  print the solution for a non-sudoku exact cover problem
    if(!isSudoku){
        std::cout << "A solution for exact cover problem has been found!\n";
        std::cout << "Solution Number: " << ++numValidSolutions;
        std::cout << "\n==================================================\n";
        for(DLXNode* solutionElement : solutionSet){
            std::cout << "Row " << solutionElement->rowID << " covers columns: ";
            std::cout << solutionElement->columnID;
            for(DLXNode* node = solutionElement->right; node != solutionElement; node = node->right){
                std::cout << ", " << node->columnID;
            }
            std::cout << std::endl;
        }
    }else if(skipOtherSolutions){
        //  TODO: define print actions for underdefined sudoku puzzle here
        //  a very simple example below
        std::cout << "It seems the sudoku puzzle is underdefined and has multiple solutions. The one printed before was one of them..." << std::endl;
    }else{
        std::cout << "A solution for the given sudoku puzzle has been found!\n"
        <<          "==========================================================\n";
        for(int row = 0; row < SUDOKU_ROWS; ++row){
            std::cout << "{";
            for(int column = 0; column < SUDOKU_COLUMNS; ++column){
                if(column == (SUDOKU_COLUMNS - 1)){
                    //  printing the last element of the row
                    std::cout << (*ptrOutputMatrix)[row][column];
                }else{
                    //  printing intermediate elements of the row
                    std::cout << (*ptrOutputMatrix)[row][column] << ", ";
                }
            }
            std::cout << "}\n";
        }
    }
} 

DLXNode* DLX::chooseColumn(){
    logFile << "Debugging line: chosseColumn() | start ...\n";
    logFile.flush();
    DLXNode* bestColumn = header->right;
    int minColSizeSoFar = INT_MAX;

    logFile << "\tNumber of visible columns = " << numColumns << std::endl;
    logFile << "\tColumn == header ? : " << (header->right == header) << std::boolalpha << std::endl;
    logFile.flush();
    for(DLXNode* column = header->right; column != header; column = column->right){
        //  determine the size of the current column
        int columnSize = 0;
        for(DLXNode* node = column->down; node != column; node = node->down){
            ++columnSize;
        }
        //  check if the current column has the least number of elements so far and has at least one element
        if(columnSize < minColSizeSoFar && columnSize > 0){
            minColSizeSoFar = columnSize;
            bestColumn = column;
        }
    }

    /*  DEBUGGING LINE : START  */
    logFile << "\tChosen column = " << bestColumn << std::endl;
    logFile.flush();
    logFile << "\tChosen column == nullptr ? : " << (bestColumn == nullptr) << std::boolalpha << std::endl;
    logFile.flush();
    logFile << "Debugging line: chosseColumn() | done\n\n";
    logFile.flush();
    /*  DEBUGGING LINE : END    */
    return bestColumn;
}

std::vector<int> DLX::calculateColumnConstraints(int row, int col, int val){
    std::vector<int> columnConstraints(NUM_SUDOKU_CONSTRAINTS);
    
    //  calculate cell constraint column number
    //  Columns [0 - 80] -> Reserved for cell constraint
    //  (row - 1) * 9 + column
    columnConstraints[0] = (row * 9) + col;

    //  calculate row constraint column number
    //  Columns [81 - 161] -> Reserved for row constraint
    //  Columns [81 - 89]: Reserved for values 1 - 9 in row 0, [90 - 98]: Reserved for values 1 - 9 in row 1, etc.
    int rowColumnsBeginning = ROW_CONSTRAINT_OFFSET + (row * 9);
    columnConstraints[1] = rowColumnsBeginning + (val - 1);

    //  calculate column constraint column number
    //  Columns [162 - 242] -> Reserved for column constraint
    //  Columns [162 - 170]: Reserved for values 1 - 9 in column 0, [171 - 180]: Reserved for values 1 - 9 in column 1, etc.
    int columColumnsBeginning = COLUMN_CONSTRAINT_OFFSET + (col * 9);
    columnConstraints[2] = columColumnsBeginning + (val - 1);

    //  calculate box constraint column number
    //  Columns [243 - 323] -> Reserved for box constraint
    //  Columns [243 - 251]: Reserved for values 1 - 9 in box 0, [252 - 260]: Reserved for values 1 - 9 in box 1, etc.
    int boxNumber = ((row / 3) * 3) + ((col / 3) * 3);
    int boxColumnsBeginning = BOX_CONSTRAINT_OFFSET + boxNumber;
    columnConstraints[3] = boxColumnsBeginning + (val - 1);

    return columnConstraints;
} 

int DLX::calculateRowPosition(int row, int col, int val){
    //  General information: Each row corresponds to a specific row-column-value combination
    //  e.g. 2nd row, 5th column (both 0 index), value 7
    //  equation: (row * 81) + (column * 9) + (value - 1)

    return (ROW_COEFFICIENT_ROW * row) + (COLUMN_COEFFICIENT_ROW * col) + (val - 1);
}

void DLX::decodePartialSolution(int dlxRow){
    int sudokuRow = dlxRow / ROW_COEFFICIENT_ROW;
    int sudokuColumn = (dlxRow - (sudokuRow * ROW_COEFFICIENT_ROW)) / COLUMN_COEFFICIENT_ROW;
    int cellValue = (dlxRow - (sudokuRow * ROW_COEFFICIENT_ROW) - (sudokuColumn  * COLUMN_COEFFICIENT_ROW)) + 1;

    (*ptrOutputMatrix)[sudokuRow][sudokuColumn] = cellValue;
}

void DLX::saveOutputMatrix(){
    for(DLXNode* partialSolution : solutionSet){
        decodePartialSolution(partialSolution->rowID);
    }
}