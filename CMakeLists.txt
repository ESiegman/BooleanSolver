cmake_minimum_required(VERSION 3.10)

# Project name
project(BooleanProcessor)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
set(SOURCES
    src/main.cpp
    src/KMapSolver.cpp
    src/BooleanProcessor.cpp
    src/Utils.cpp
)

# Add header files
set(HEADERS
    src/KMapSolver.hpp
    src/BooleanProcessor.hpp
    src/Utils.hpp
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
