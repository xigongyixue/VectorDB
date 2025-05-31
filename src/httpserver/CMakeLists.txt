cmake_minimum_required(VERSION 3.20)

project(httpserver)

set(LIBRARY_OUTPUT_PATH ${LIB_PATH})
aux_source_directory(./ SRC)
add_library(${HTTP_SERVER} SHARED ${SRC})
target_link_libraries(${HTTP_SERVER} ${DATABASE})