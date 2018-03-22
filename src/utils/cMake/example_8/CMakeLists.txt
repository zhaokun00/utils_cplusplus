
cmake_minimum_required(VERSION 2.8.7)

project(TestExample)

set(SRC_LIST TestExample8.cpp)

#add_definitions:为源文件的编译添加由-D引入的宏定义
#格式:add_definitions(-DFOO -DBAR ...)
add_definitions(-DTEST1 -DTEST2=0)

add_executable(TestExample8 ${SRC_LIST})

