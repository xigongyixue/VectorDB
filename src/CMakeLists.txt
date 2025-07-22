cmake_minimum_required(VERSION 3.20)

project(src)

set(LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# 头文件
set(HEAD_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${HEAD_PATH})

# 链接库
set(LOGGER logger)
set(ROARING roaring)
set(INDEX index)
set(HTTP_SERVER httpserver)
set(PERSISTENCE persistence)
set(DATABASE database)
set(NURAFT_ nuraft_)

add_subdirectory(roaring)
add_subdirectory(index)
add_subdirectory(logger)
add_subdirectory(persistence)
add_subdirectory(database)
add_subdirectory(nuraft)
add_subdirectory(httpserver)
add_subdirectory(vdbserver)