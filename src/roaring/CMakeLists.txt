cmake_minimum_required(VERSION 3.20)

project(roaring)

set(LIBRARY_OUTPUT_PATH ${LIB_PATH})
add_library(${ROARING} SHARED roaring.c)