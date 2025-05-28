cmake_minimum_required(VERSION 3.20)

project(database)

set(LIBRARY_OUTPUT_PATH ${LIB_PATH})
aux_source_directory(./ SRC)
add_library(${DATABASE} SHARED ${SRC})