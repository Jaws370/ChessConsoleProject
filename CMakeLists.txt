cmake_minimum_required(VERSION 3.10)
project(ChessConsoleProject)

# set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")

# create executable
add_executable(ChessConsoleProject ${SOURCES})

# copy assets to build directory if you have any
add_custom_command(
  TARGET ChessConsoleProject
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)
