
cmake_minimum_required(VERSION 2.8.7)

project(TestExample)

set(SRC_LIST TestExample4.cpp)

add_executable(TestExample4 ${SRC_LIST})

#target_link_libraries:给指定的库链接到一个目标文件上
#格式:target_link_libraries(<target> ... <item>... ...)
target_link_libraries(TestExample4 pthread)