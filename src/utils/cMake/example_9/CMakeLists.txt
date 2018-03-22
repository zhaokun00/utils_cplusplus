
cmake_minimum_required(VERSION 2.8.7)

project(TestExample)

#CMake咋ubuntu系统下默认使用系统的gcc、g++编译器,编译arm下的程序要使用arm-linux-gcc或者arm-linux-g++,因此需要对CMake进行设置(通过在CMakeLists.txt中指定交叉编译器的方法)

#在CMakeLists.txt开始加入相关的设置

#必须配置:告知当前使用的是交叉编译方式
SET(CMAKE_SYSTEM_NAME Linux)

#必须配置:指定C交叉编译器或者交叉编译器的绝对地址
#SET(CMAKE_C_COMPILER "arm-linux-gnueabi-gcc")

#必须配置:指定C++交叉编译器
SET(CMAKE_CXX_COMPILER "arm-linux-gnueabi-g++")

#不一定需要设置

#指定交叉编译环境安装目录...
#SET(CMAKE_FIND_ROOT_PATH "...")

#从来不在指定目录下查找工具程序
#SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

#只在指定目录下查找库文件
#SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)

#只在指定目录下查找头文件
#SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(SRC_LIST TestExample9.cpp)

add_executable(TestExample9 ${SRC_LIST})

