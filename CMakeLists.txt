cmake_minimum_required(VERSION 3.17)
project(GridSimulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_DEBUG")


add_executable(GridSimulator main.cpp parser.cpp adjMatrix.cpp)
target_link_libraries(GridSimulator -fsanitize=address)