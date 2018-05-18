cmake_minimum_required(VERSION 2.8.7)

project(zgateway)

#设置编译的平台

#设置X86_64平台
SET(PLATFORM "X86_64")

#设置极路由的linux交叉编译环境
#SET(PLATFORM "Linux_JiLuYou")

if (${PLATFORM} STREQUAL "X86_64")

    #找到lua的库
    find_package(Lua 5.3 REQUIRED)

    #配置头文件
    include_directories(
            ${LUA_INCLUDE_DIR}
    )

    #配置库文件
    link_directories(
            ${CMAKE_SOURCE_DIR}/lib/X86_64
    )

elseif (${PLATFORM} STREQUAL "Linux_JiLuYou")

else ()
    message(FATAL_ERROR "请先设置编译的环境,设置开始的PLATFORM变量")
endif ()

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

#设置程序编译的版本
SET(CMAKE_BUILD_TYPE "Release")

#指定头文件目录
include_directories(
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/detail
)

#配置MQTT支持ssl协议,如果不配置该选项,MQTT不支持ssl协议,编译能通过,但是再运行时会报错
add_definitions(-DOPENSSL=1)

#添加源文件
aux_source_directory(. app)

#添加变量root、src到变量SRC_LIST,变量使用{}括起来,而不能使用()括起来
set(SRC_LIST ${app})

add_executable(zgateway ${SRC_LIST})

target_link_libraries(zgateway pthread lua m rt dl curl crypto ssl )


