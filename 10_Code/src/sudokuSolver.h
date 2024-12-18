#pragma once
#include <cstdint>
#include <vector>
#include <cstdint>
#include <stdexcept>

#define NUM_ROWS 9
#define NUM_COLUMNS 9
#define NUM_NINTHS 9
#define SUM_FINISHED_SUDOKU 405

constexpr int NUM_POSSIBILITIES = 9;
constexpr uint16_t BITPOS_1 = 0b0000000000000001;
constexpr uint16_t BITPOS_2 = 0b0000000000000010;
constexpr uint16_t BITPOS_3 = 0b0000000000000100;
constexpr uint16_t BITPOS_4 = 0b0000000000001000;
constexpr uint16_t BITPOS_5 = 0b0000000000010000;
constexpr uint16_t BITPOS_6 = 0b0000000000100000;
constexpr uint16_t BITPOS_7 = 0b0000000001000000;
constexpr uint16_t BITPOS_8 = 0b0000000010000000;
constexpr uint16_t BITPOS_9 = 0b0000000100000000;
constexpr uint16_t ALLBITS = 0b0000000111111111;
constexpr uint16_t MARKER_BIT = 0b1000000000000000;
constexpr uint16_t VALUE_FOUND_BIT = 0b0100000000000000;

constexpr uint16_t ROW1 = 0b0000000000000001;
constexpr uint16_t ROW2 = 0b0000000000000010;
constexpr uint16_t ROW3 = 0b0000000000000100;
constexpr uint16_t ROW4 = 0b0000000000001000;
constexpr uint16_t ROW5 = 0b0000000000010000;
constexpr uint16_t ROW6 = 0b0000000000100000;
constexpr uint16_t ROW7 = 0b0000000001000000;
constexpr uint16_t ROW8 = 0b0000000010000000;
constexpr uint16_t ROW9 = 0b0000000100000000;

constexpr uint16_t COLUMN1 = 0b0000000000000001;
constexpr uint16_t COLUMN2 = 0b0000000000000010;
constexpr uint16_t COLUMN3 = 0b0000000000000100;
constexpr uint16_t COLUMN4 = 0b0000000000001000;
constexpr uint16_t COLUMN5 = 0b0000000000010000;
constexpr uint16_t COLUMN6 = 0b0000000000100000;
constexpr uint16_t COLUMN7 = 0b0000000001000000;
constexpr uint16_t COLUMN8 = 0b0000000010000000;
constexpr uint16_t COLUMN9 = 0b0000000100000000;

constexpr uint16_t SUBGRID1 = 0b0000000000000001;
constexpr uint16_t SUBGRID2 = 0b0000000000000010;
constexpr uint16_t SUBGRID3 = 0b0000000000000100;
constexpr uint16_t SUBGRID4 = 0b0000000000001000;
constexpr uint16_t SUBGRID5 = 0b0000000000010000;
constexpr uint16_t SUBGRID6 = 0b0000000000100000;
constexpr uint16_t SUBGRID7 = 0b0000000001000000;
constexpr uint16_t SUBGRID8 = 0b0000000010000000;
constexpr uint16_t SUBGRID9 = 0b0000000100000000;

using BoardType = std::vector<std::vector<int>>;
using BoardPosType = std::vector<std::vector<uint16_t>>;

struct BoardIndexRange
{
    int rowFirst;  //  first row index for one ninth of board
    int rowLast;  //  last row index for one ninth of board
    int columnFirst; //  first column index for one ninth of board
    int columnLast; //  last column index for one ninth of board
};

//  This struct will be used to hold indexes of unknown values
struct IndexUnknownVals{
    int row;
    int column;
};

class sudokuSolver{
    public:
        sudokuSolver(BoardType &board, bool isSolved);
        sudokuSolver();

        ~sudokuSolver();

        //  This function is for solving a non-finished sudoku
        BoardType solvePuzzle(BoardType &Board);

        //  This function is for completed sudokus, whose correctness is checked
        bool checkPuzzle();

    protected:
        BoardType _board;
        BoardPosType _possibleValues;

        bool _isSolved;
        bool _originalsMarked;

        uint16_t _bufEliminatedVals;
        uint16_t _completedRows;
        uint16_t _completedColumns;
        uint16_t _completedOneNinths;
        
        std::vector<IndexUnknownVals> _indexVecUnknownVals;
        int _nextElementUnknownValsVec;

        
        //  This function returns vector index of a target element whose row and column is known
        void getIndexRange(BoardIndexRange &indexRange, int boardOneNinth);

        //  This function marks elements from sudoku itself, such that they cannot be changed
        void markOriginals();

        //  This function checks whether an element is an original element or a solved element
        bool isOriginal(int row, int column);

        //  This function checks whether an unknown element found
        bool isFound(int row, int column);

        //  This function sets a possible value to a given square
        void setPossibleValue(int row, int column, int value);

        //  This function removes an impossible value from a given square
        void removeImpossibleValue(int row, int column, int value);

        //  This function interpretes the bit position and return the real int value
        void getIntValueFromBit(int &interprtdIntVal, uint16_t bitPos);

        //  This function checks whether a value is known, and if so, what is the value
        int isValueKnown(int row, int column);

        //  This function checks whether an unknown value has been found, edits _board and returns its value
        int isValueFound(int row, int column);

        //  This function checks one-ninth regions and marks possible values to empty squares
        void checkOneNinths();

        //  This function checks rows and marks possible values to empty squares
        void checkRows();

        //  This function checks columns and marks possible values to empty squares
        void checkColumns();
};


//  Project specific exception classes
class IllegalOperationException : public std::exception{
    public:
        explicit IllegalOperationException(const std::string& message) : _message(message){
        }
        
        const char * what() const noexcept override{
            return _message.c_str();
        }
    private:
        std::string _message;
};

class LogicalErrorOccured : public std::exception{
    public:
        explicit LogicalErrorOccured(const std::string& message) : _message(message){
        }
        
        const char * what() const noexcept override{
            return _message.c_str();
        }
    private:
        std::string _message;
};