# SudokuSolver Project
This project implements a sudoku solver program based on Donald Knuth's DLX (Dancing Links algorithm)

**Developed by:** Levent Bilal

## Table of contents
1. [Motivation](#motivation)
2. [Challanges](#challanges)

    2.1. [Main challenge](#main-challenge)

    2.2. [Additional challenges](#additional-challenges)


3. [Materials and methods](#materials-and-methods)
    
    3.1. [Exact cover problem](#exact-cover-problem)
    
    3.2. [Dancing links algorithm to solve exact cover problems](#dancing-links-algorithm-to-solve-exact-cover-problems)

    3.3. [Adaptation of exact cover problems to sudoku puzzles](#adaptation-of-exact-cover-problems-to-sudoku-puzzles)

    3.4. [Adaptation of dancing links algorithm to solve sudoku exact cover problems](#adaptation-of-dancing-links-algorithm-to-solve-sudoku-exact-cover-problems)

4. [How to use this code](#how-to-use-this-code)

## Motivation

This project is product of a coding challenge organised between automotive software developer collegues. 
The aim is to develop a sudoku solver program capable of solving fully defined as well as underdefined 
sudoku puzzles in the shortest time possible.

After the competition, the code was published as an open source project. It is open to use for everyone who 
wishes to cheat in sudoku every now and then, as well as for those who wish to see a Dancing Links algorithm 
implementation in C++.

## Challanges

The original challanges for the competition are descriped in [SudokuChallenge.pdf](SodokuChallenge.pdf). There, the main format
for the project has been defined. After awhile, some additional challanges, described in [SudokuChallenge_AdditionalTask](SudokuChallange_AdditionalTask.txt), 
have been added to the challenge. This addition mainly includes sudoku format and main program arguments. A remarkable addition to the main challenge is handling of underdefined sudokus.

### Main challenge

As defineded above, the main challenge is solving a sudoku, defined in a 2D vector. This vector should contain integer values varying from 0 to 9, values 1 - 9 representing sudoku clues and 0 representing empty sudoku cells.

Some other rules include:

- Program should be developed in an object-oriented programming language, such as C++, Python, Java, etc.
- The solution should be accurate.
- Program should solve sudokus as fast as possible, and the participant who develops the fastest solver, wins the contest.
- The code should be structured well, be readable and clean.

### Additional challenges

Additional challenges mainly include the following:

- Sudoku puzzle should be given to the program as main program argument.
- The format of sudoku puzzle include a 1D vector with 81 elements, each representing the corresponding sudoku element, such as [12004...79].
- An example of calling the program is given below:
    ``` bash
    main.exe 070001903002600000000807064000003090201000608960100000640710385008340000723590016
    ```
- Sudoku solver program should be able to recognize underdefined sudokus which does not have enough clue to be solved in a unique way. In this case, program should find be able to one of many possible solutions.

## Materials and methods

### Exact cover problem

### Dancing links algorithm to solve exact cover problems

### Adaptation of exact cover problems to sudoku puzzles

### Adaptation of dancing links algorithm to solve sudoku exact cover problems

## How to use this code