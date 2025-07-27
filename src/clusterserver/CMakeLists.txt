cmake_minimum_required(VERSION 3.20)

project(clusterserver)

set(CURL curl)
set(CPPREST cpprest)
set(ETCD etcd-cpp-api)
set(LIBRARY_OUTPUT_PATH ${LIB_PATH})
aux_source_directory(./ SRC)
add_library(${CLUSTER_SERVER} SHARED ${SRC})
target_link_libraries(${CLUSTER_SERVER} ${CURL} ${CPPREST} ${ETCD})