cmake_minimum_required(VERSION 3.10)

# Project name
project(KMapSolver)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
set(SOURCES
    src/main.cpp
    src/KMapSolver.cpp
)

# Add header files
set(HEADERS
    src/KMapSolver.hpp
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
