
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

aux_source_directory(. root)
aux_source_directory(./fun fun)

#将所有的源文件全部存储在一个统一的变量中,在set里面换行直接回车换行即可,不用添加字符\
set(SRC_LIST
    ${root}
	${fun})

add_executable(TestExample3 ${SRC_LIST})

