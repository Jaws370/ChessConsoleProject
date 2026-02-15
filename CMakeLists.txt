cmake_minimum_required(VERSION 3.10)
project(ChessConsoleProject)

# set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add source files
set(SOURCES src/main.cpp src/board/Board.cpp)

# create executable
add_executable(ChessConsoleProject ${SOURCES})

# include header files
target_include_directories(ChessConsoleProject PRIVATE ${CMAKE_SOURCE_DIR}/src)

# copy assets to build directory if you have any
add_custom_command(
        TARGET ChessConsoleProject
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)
