
cmake_minimum_required(VERSION 2.8.7)

project(TestExample)

include_directories(                                                                                                                                                                  
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#link_directories:指定链接器查找的库的路径
#格式:link_directories(directory1 directory2 ...)

link_directories(
  ./lib
)
set(SRC_LIST TestExample6.cpp)

add_executable(TestExample6 ${SRC_LIST})

target_link_libraries(TestExample6 hello)