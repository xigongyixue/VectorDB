cmake_minimum_required(VERSION 3.20)

project(persistence)

set(LIBRARY_OUTPUT_PATH ${LIB_PATH})
add_library(${PERSISTENCE} SHARED persistence.cpp)