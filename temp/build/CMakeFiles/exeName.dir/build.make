# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.31

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\levent.bilal\OneDrive - intive\Desktop\SudokuSolverProject\temp"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\levent.bilal\OneDrive - intive\Desktop\SudokuSolverProject\temp\build"

# Include any dependencies generated for this target.
include CMakeFiles/exeName.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/exeName.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/exeName.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/exeName.dir/flags.make

CMakeFiles/exeName.dir/codegen:
.PHONY : CMakeFiles/exeName.dir/codegen

CMakeFiles/exeName.dir/main.cc.obj: CMakeFiles/exeName.dir/flags.make
CMakeFiles/exeName.dir/main.cc.obj: C:/Users/levent.bilal/OneDrive\ -\ intive/Desktop/SudokuSolverProject/temp/main.cc
CMakeFiles/exeName.dir/main.cc.obj: CMakeFiles/exeName.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="C:\Users\levent.bilal\OneDrive - intive\Desktop\SudokuSolverProject\temp\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/exeName.dir/main.cc.obj"
	C:\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/exeName.dir/main.cc.obj -MF CMakeFiles\exeName.dir\main.cc.obj.d -o CMakeFiles\exeName.dir\main.cc.obj -c "C:\Users\levent.bilal\OneDrive - intive\Desktop\SudokuSolverProject\temp\main.cc"

CMakeFiles/exeName.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/exeName.dir/main.cc.i"
	C:\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\levent.bilal\OneDrive - intive\Desktop\SudokuSolverProject\temp\main.cc" > CMakeFiles\exeName.dir\main.cc.i

CMakeFiles/exeName.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/exeName.dir/main.cc.s"
	C:\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\levent.bilal\OneDrive - intive\Desktop\SudokuSolverProject\temp\main.cc" -o CMakeFiles\exeName.dir\main.cc.s

# Object files for target exeName
exeName_OBJECTS = \
"CMakeFiles/exeName.dir/main.cc.obj"

# External object files for target exeName
exeName_EXTERNAL_OBJECTS =

exeName.exe: CMakeFiles/exeName.dir/main.cc.obj
exeName.exe: CMakeFiles/exeName.dir/build.make
exeName.exe: CMakeFiles/exeName.dir/linkLibs.rsp
exeName.exe: CMakeFiles/exeName.dir/objects1.rsp
exeName.exe: CMakeFiles/exeName.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="C:\Users\levent.bilal\OneDrive - intive\Desktop\SudokuSolverProject\temp\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable exeName.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\exeName.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/exeName.dir/build: exeName.exe
.PHONY : CMakeFiles/exeName.dir/build

CMakeFiles/exeName.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\exeName.dir\cmake_clean.cmake
.PHONY : CMakeFiles/exeName.dir/clean

CMakeFiles/exeName.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\levent.bilal\OneDrive - intive\Desktop\SudokuSolverProject\temp" "C:\Users\levent.bilal\OneDrive - intive\Desktop\SudokuSolverProject\temp" "C:\Users\levent.bilal\OneDrive - intive\Desktop\SudokuSolverProject\temp\build" "C:\Users\levent.bilal\OneDrive - intive\Desktop\SudokuSolverProject\temp\build" "C:\Users\levent.bilal\OneDrive - intive\Desktop\SudokuSolverProject\temp\build\CMakeFiles\exeName.dir\DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/exeName.dir/depend

