cmake_minimum_required(VERSION 3.20)

project(test)

find_package(faiss REQUIRED)

link_directories(${LIB_PATH})

set(FAISS faiss)
set(ALL_LIB ${LOGGER} ${INDEX} ${HTTP_SERVER} ${FAISS})

add_executable(main main.cpp)
target_link_libraries(main ${ALL_LIB})