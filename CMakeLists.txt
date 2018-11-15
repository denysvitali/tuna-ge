cmake_minimum_required(VERSION 3.12)
project(tuna_ge)

set(CMAKE_CXX_STANDARD 14)

add_library(tuna_ge library.cpp include/library.h)