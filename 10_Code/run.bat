@echo off

::  Check that the executable exists
if exist "build\app\SudokuSolverApp.exe" (
    ::  Run the executable if exists
    build\app\SudokuSolverApp.exe
) else (
    echo Executable not found. Please compile your project first
)