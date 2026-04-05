cmake_minimum_required(VERSION 3.10)
project(ChessLib)

# set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add source files
set(SOURCES src/chess.cpp src/game_data.cpp tests/test_chess.cpp)

# create executable
add_executable(ChessLib ${SOURCES})

# include header files
target_include_directories(ChessLib PRIVATE ${CMAKE_SOURCE_DIR}/include)
