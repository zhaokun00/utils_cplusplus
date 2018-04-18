cmake_minimum_required(VERSION 2.8.7)

project(TestExample)

#指定头文件目录
include_directories(
  ${CMAKE_SOURCE_DIR}
)

#指定库文件目录
link_directories(
  ${CMAKE_SOURCE_DIR}/lib
)

#add_compile_options(-std=c++11)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

#添加源文件
#添加当前目录下的所有源文件到变量root中
aux_source_directory(. root)

#添加变量root、src到变量SRC_LIST
set(SRC_LIST ${root})

add_executable(queue ${SRC_LIST})

#添加依赖的库
target_link_libraries(queue pthread)