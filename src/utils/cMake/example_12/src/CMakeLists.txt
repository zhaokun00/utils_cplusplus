
cmake_minimum_required(VERSION 2.8.7)

project(TestExample)

#include_directories:将给定的目录添加到编译器中,用于搜索包含文件的目录,相对路径则是相对于当前源码的目录
#格式:include_directories([AFTER|BEFORE] [SYSTEM] dir1 [dir2 ...])
#即指定头文件的目录
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/fun
)

#指定查找库文件的路径
link_directories(
	  .
	)

aux_source_directory(. root)

#所有的源文件全部存储在一个统一的变量中,在set里面换行直接回车换行即可,不用添加字符
set(SRC_LIST
    ${root}
	)

#配置可执行文件及库文件的输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(TestExample7 ${SRC_LIST})

target_link_libraries(TestExample7 fun)
