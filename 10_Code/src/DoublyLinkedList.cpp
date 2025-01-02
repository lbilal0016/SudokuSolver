#include "DoublyLinkedList.h"

DLX::DLX(const std::vector<std::vector<int>>& matrix){
    int numColumns = matrix[0].size();
    header = new DLXNode();
    columnHeaders.resize(numColumns);

    //  create column headers
    DLXNode* prev = header;
    for(int i = 0; i < numColumns; ++i){
        DLXNode* col = new DLXNode(COLUMN_HEADER_ROW,i);
        columnHeaders[i] = col;
        prev->right = col;
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
    std::cout << "Column Headers:" << std::endl;
    for (DLXNode* col = header->right; col != header; col = col->right) {
        std::cout << "Column " << col->columnID << " (Row " << col->rowID << ")" << std::endl;
    }

    // Print rows
    std::cout << "Rows:" << std::endl;
    for (DLXNode* col = header->right; col != header; col = col->right) {
        for (DLXNode* row = col->down; row != col; row = row->down) {
            std::cout << "Row " << row->rowID << " in Column " << row->columnID << std::endl;
        }
    }
}