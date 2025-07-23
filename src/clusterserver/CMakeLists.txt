cmake_minimum_required(VERSION 3.20)

project(clusterserver)

set(LIBRARY_OUTPUT_PATH ${LIB_PATH})
aux_source_directory(./ SRC)
add_library(${CLUSTER_SERVER} SHARED ${SRC})