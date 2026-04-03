cmake_minimum_required(VERSION 4.2)
project(GameWatchCon)

set(CMAKE_CXX_STANDARD 23)

add_executable(GameWatchCon Code/Core/Source/main.cpp
        Code/Core/Source/Console.cpp)

include_directories(Code/Core/Include Code/Interfaces/Include)