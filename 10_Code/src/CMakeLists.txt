#   Find all library files
file(GLOB SOURCE_FILES "*.cpp")

#   Add found library files to CMakeLists
add_library(${LIBRARY_NAME} ${SOURCE_FILES})

#   Add library files in include directories
target_include_directories(${LIBRARY_NAME} PUBLIC "./")