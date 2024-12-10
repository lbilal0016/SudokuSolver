@echo off

if exist build\Makefile (
    echo Makefile found. Compilation started.
    cd build
    cmake --build .
) else (
    echo Makefile not found. Please create Makefile using cmake first
)