cmake_minimum_required(VERSION 3.20)

project(logger)

set(LIBRARY_OUTPUT_PATH ${LIB_PATH})
add_library(${LOGGER} SHARED logger.cpp)