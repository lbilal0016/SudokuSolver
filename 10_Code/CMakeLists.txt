cmake_minimum_required(VERSION 3.20)

project(SudokuSolver VERSION 1.0.0 LANGUAGES C CXX)

set(EXECUTABLE_NAME SudokuSolverApp)
set(LIBRARY_NAME SudokuLib)

add_subdirectory(src)
add_subdirectory(app)