
cmake_minimum_required(VERSION 2.8.7)

project(TestExample)

#CMAKE_CURRENT_SOURCE_DIR:指正在被处理的源码目录的路径
message(WARNING ${CMAKE_CURRENT_SOURCE_DIR})

#include_directories:将给定的目录添加到编译器中,用于搜索包含文件的目录,相对路径则是相对于当前源码的目录
#格式:include_directories([AFTER|BEFORE] [SYSTEM] dir1 [dir2 ...])
#即指定头文件的目录
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/fun
)

#第一种方式:直接手写添加的源码
#缺点:源文件越多,需要添加次数就越多,而且每增加一个源文件就需要修改CMakeLists.txt文件,耦合性太大
#set(SRC_LIST TestExample2.cpp hello.cpp fun/fun.cpp)

#第二种方式:添加本层目录中所有源文件
#aux_source_directory:查找目录中的所有源文件
#格式:aux_source_directory(<dir> <variable>),查找指定目录dir中所有源文件的名称,并将列表存储在提供的variable中
#目录后面不用再添加/,例如./fun/
aux_source_directory(. root)
aux_source_directory(./fun fun)

#将所有的源文件全部存储在一个统一的变量中,在set里面换行直接回车换行即可,不用添加字符\
set(SRC_LIST
    ${root}
	${fun})

add_executable(TestExample2 ${SRC_LIST})

