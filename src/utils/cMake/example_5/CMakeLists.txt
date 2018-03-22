
cmake_minimum_required(VERSION 2.8.7)

project(TestLibrary)

include_directories(                                                                                                                                                  
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(SRC_LIST hello.cpp)

#add_library:使用指定的源文件给项目添加一个库
#格式:add_library(<name> [STATIC | SHARED | MODULE] [EXCLUDE_FROM_ALL] source1 [source2 ...])
#name:该命令列出的源文件来构建的库目标,对应于逻辑目标名称,在项目中必须是全局唯一的
#STATIC:静态库,在链接其他目标时使用
#SHARED:动态链接库,运行时加载
#MODULE:不会被链接到其他目标中,但是可能会在运行时用dlopen-系列的函数动态库链接

#静态库与动态库不能一块执行,原因在于name只能是全局唯一的

#生成静态库
#add_library(hello STATIC ${SRC_LIST})

#生成动态库
add_library(hello SHARED ${SRC_LIST})

#这种同时生成静态库和动态库的方式不能编译通过
#add_library(hello SHARED | STATIC ${SRC_LIST})
