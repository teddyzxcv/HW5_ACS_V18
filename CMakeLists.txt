cmake_minimum_required(VERSION 3.0)
project(HW5_ACS_V18)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS -pthread)


add_executable(HW5_ACS_V18 main.cpp Talker.cpp Talker.h Call.cpp Call.h)
