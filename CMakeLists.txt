cmake_minimum_required(VERSION 3.11)
project(Zero)

set(CMAKE_CXX_STANDARD 17)

add_executable(Zero main.cpp Zero.cpp)
target_link_libraries(Zero libstdc++fs.a)