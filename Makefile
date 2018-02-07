###############################################################################
#
# Generic Makefile for C/C++ Program
#
# Author: zhaokun
# Date:   2018/01/30
#
# Description:
# 多目录多文件编译可执行程序的Makefile编写框架
#
# Usage:
#   $ make           compile and link the program.
#   $ make clean     clean the objectives and dependencies.
###############################################################################

#定义源文件根目录
DIR_SRC = ./src
#定义临时文件(.o)根目录
DIR_OBJ = ./obj
#定义可执行文件的根目录
DIR_BIN = ./bin

#定义源文件路径
#@该处需要修改@#
FILE_SRC = ${DIR_SRC}/*.cpp \
		   ${DIR_SRC}/utils/criticalSectionPosix/*.cpp \
		   ${DIR_SRC}/utils/ringBuf/*.cpp \
		   ${DIR_SRC}/utils/eventPosix/*.cpp \
		   ${DIR_SRC}/utils/threadPosix/*.cpp \
		   ${DIR_SRC}/utils/dataTag/*.cpp \
		   ${DIR_SRC}/utils/cJson/*.cpp \
		   
		   
#定义头文件路径
#@该处需要修改@#
FILE_INC = -I./include \
		   -I./src/utils/log \
		   -I./src/utils/criticalSectionPosix \
		   -I./src/utils/ringBuf \
		   -I./src/utils/eventPosix \
		   -I./src/utils/threadPosix \
		   -I./src/utils/dataTag \
		   -I./src/utils/cJson

#定义依赖的库文件路径
#@该处需要修改@#
FILE_LIB = -L./

#定义依赖的动态库
#@该处需要修改@#
LDFLAGS = -lpthread -lm

#在Makefile中打印警告信息或者错误信息的方法
#警告信息:$(warning xxxxx),$(warning     ${XXX}),示例:$(warning "zhaokun"),$(warning ${DIR_SRC} ${DIR_OBJ})
#错误信息:$(error xxxxx),$(error     ${XXX})

#wildcard:扩展通配符
SRC = $(wildcard ${FILE_SRC})
#$(warning ${SRC}),输出结果:./src/main.cpp ./src/head/head.cpp ./src/test/test.cpp

#notdir:去掉路径
#patsubst:替换通配符
OBJ = $(patsubst %.cpp,${DIR_OBJ}/%.o,$(notdir ${SRC})) 
#$(warning $(notdir ${SRC})),输出结果:main.cpp head.cpp test.cpp
#$(warning ${OBJ}),输出结果:/obj/main.o ./obj/head.o ./obj/test.o

#定义最终生成可执行文件的名字
TARGET = main

#定义生成可执行文件的路径
BIN_TARGET = ${DIR_BIN}/${TARGET}

#定义编译器
CC = g++

#定义一个编译选项
#这里只配置头文件的路径,不用配置库文件的路径
CFLAGS = -g -Wall ${FILE_INC}

#编译生成的最终目标
#注意事项:$(FILE_LIB) $(LDFLAGS)变量一定要放在$(OBJ)后面
${BIN_TARGET}:${OBJ}
	$(CC) $(OBJ) $(FILE_LIB) $(LDFLAGS) -o $@

#以下为将来如果添加文件时需要添加的选项
#@该处需要修改@#
${DIR_OBJ}/%.o:${DIR_SRC}/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

${DIR_OBJ}/%.o:${DIR_SRC}/utils/criticalSectionPosix/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

${DIR_OBJ}/%.o:${DIR_SRC}/utils/ringBuf/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

${DIR_OBJ}/%.o:${DIR_SRC}/utils/eventPosix/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

${DIR_OBJ}/%.o:${DIR_SRC}/utils/threadPosix/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

${DIR_OBJ}/%.o:${DIR_SRC}/utils/dataTag/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

${DIR_OBJ}/%.o:${DIR_SRC}/utils/cJson/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -rf bin/* obj/*
