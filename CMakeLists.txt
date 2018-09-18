cmake_minimum_required(VERSION 3.12)
project(NFSSP)

set(CMAKE_CXX_STANDARD 17)

add_executable(NFSSP main.cpp schedule.cpp schedule.h constructiveHeuristics.cpp constructiveHeuristics.h)