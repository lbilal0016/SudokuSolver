#include "DoublyLinkedList.h"

DLX::DLX(const std::vector<std::vector<int>>& matrix){
    numValidSolutions = 0;
    int numColumns = matrix[0].size();
    header = new DLXNode();
    columnHeaders.resize(numColumns);

    //  create column headers
    DLXNode* prev = header;
    for(int i = 0; i < numColumns; ++i){
        DLXNode* col = new DLXNode(COLUMN_HEADER_ROW,i);
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

DLX::DLX(const std::vector<std::vector<int>>& matrix, bool isSudokuFlag){
    //  safety mechanism: false function call
    if(!isSudokuFlag){
        std::cerr << "Faulty function call, if you wish to solve a sudoku puzzle, second argument should be true\n"
        << "Exiting program ...\n";
    }

    //  safety mechanism: wrong matrix size
    if(matrix.size() != SUDOKU_ROWS || matrix[0].size() != SUDOKU_COLUMNS){
        std::cerr << "Provided input matrix does not correspond to a standard sudoku grid.\n"
        << "Exiting program ...\n";
    }



    //  create the starting point for DLX structure
    header = new DLXNode();
    columnHeaders.resize(SUDOKU_COLUMN_CONSTRAINTS);

    //  create column headers
    DLXNode* prev = header;
    for(int i = 0; i < SUDOKU_COLUMN_CONSTRAINTS; ++i){
        DLXNode* col = new DLXNode(COLUMN_HEADER_ROW,i);
        columnHeaders[i] = col;
        prev->right = col;
        prev->column = prev;
        col->left = prev;
        prev = col;
    }
    prev->right = header;
    header->left = prev;

    for(int i : matrix){
        for(int j : matrix[i]){
            if(matrix[i][j] == 0){
                //  there is no clue in this coordinate, skip
                continue;
            }else if(matrix[i][j] < 0 || matrix[i][j] > 9){
                //  error: clue is invalid for a standard sudoku problem
                std::cerr << "Element[" << i << ", " << j << "] is not a valid number for a sudoku puzzle.\n"
                << "Exiting program ...\n";
            }

            //  add corresponding row to exact cover matrix with proper columns are equal to 1
            addRow(calculateRowPosition(i, j, matrix[i][j]), calculateColumnConstraints(matrix, i, j));
        }
    }
}

void DLX::addRow(int rowID, const std::vector<int>& columns){
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
    /*  Debugging line:
    std::cout << "Search depth: " << searchDepth << std::endl;
    */

    //  Check if a solution has already been found
    if(header->right == header){
        printSolution();    //  TODO: Not implemented yet!
        //  if all columns were covered successfully, function returns here, indicating a valid solution
        return;
    }

    DLXNode* column = chooseColumn();//header->right;    //  TODO: Not implemented yet!
    
    for(DLXNode* row = column->down; row != column; row = row->down){
        /*  Debugging line:
        std::cout << "Selected column: " << column->columnID << std::endl;
        */

        //  add this row to the solution set, assuming it is a valid solution
        solutionSet.push_back(row);
        //  cover the column in which solution is assumed
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

void DLX::coverColumn(DLXNode* column){
    column->left->right = column->right;
    column->right->left = column->left;

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
}

void DLX::uncoverColumn(DLXNode* column){
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
}

void DLX::printSolution(){
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
} //  TODO

DLXNode* DLX::chooseColumn(){
    DLXNode* bestColumn = nullptr;
    int minColSizeSoFar = INT_MAX;

    for(DLXNode* column = header->right; column != header; column = column->right){
        //  determine the size of the current column
        int columnSize = 0;
        for(DLXNode* node = column->down; node != column; node = node->down){
            ++columnSize;
        }
        //  check if the current column has the least number of elements so far
        if(columnSize < minColSizeSoFar){
            minColSizeSoFar = columnSize;
            bestColumn = column;
        }
    }
    return bestColumn;
}

