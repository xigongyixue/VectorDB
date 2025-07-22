cmake_minimum_required(VERSION 3.20)

project(test)

find_package(faiss REQUIRED)

link_directories(${LIB_PATH})

set(FAISS faiss)
set(ROCKSDB rocksdb)
set(NURAFT nuraft) # 系统库
set(THIRD_LIB ${FAISS} ${ROCKSDB} ${NURAFT})

set(MY_LIB ${LOGGER} ${INDEX} ${HTTP_SERVER} ${DATABASE} ${NURAFT_})

set(ALL_LIB ${MY_LIB} ${THIRD_LIB})

add_executable(vdb_server vdb_server.cpp)
target_link_libraries(vdb_server ${ALL_LIB})