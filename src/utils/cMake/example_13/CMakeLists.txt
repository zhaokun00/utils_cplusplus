#cmake中不区分大小写

#message:向用户显示消息
#格式:message([<mode>] "message to display" ...)
#mode:可选的值为none，STATUS，WARNING，AUTHOR_WARNING，SEND_ERROR，FATAL_ERROR，DEPRECATION
message(STATUS "cmake start")

#一般工程都是从cmake_minimum_required开始的,表示系统cmake的最低版本,如果版本比这个小,那么无法cmake通过
cmake_minimum_required(VERSION 2.8.7)

#project:为整个工程设置一个工程名
#格式:project(<PROJECT-NAME> [LANGUAGES] [<language-name>...])
project(TestExample)

#set:将一个CMAKE变量设置为给定值
#格式:set(<variable> <value>... [PARENT_SCOPE])
#设置源文件,将SRC_LIST变量设置成源文件
set(SRC_LIST TestExample1.cpp)

#指定输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)

#将文件复制到其他位置并修改其内容
#文件格式:configgure_file(<input> <output>)
configure_file(${CMAKE_SOURCE_DIR}/gateway.conf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config/gateway.conf COPYONLY)

#下面这种形式将gateway.conf复制成了config文件
#configure_file(${CMAKE_SOURCE_DIR}/gateway.conf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config/ COPYONLY)

#add_executable:使用指定的源文件给项目添加一个可执行文件
#格式:add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL] source1 [source2 ...])
#生成可执行程序TestExample1,${}表示引用某个值
add_executable(TestExample1 ${SRC_LIST})

message(STATUS "cmake end")
