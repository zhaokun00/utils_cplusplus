#ifndef LOG_H____
#define LOG_H____

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>  
#include <sys/time.h> 
	
#define LOG_LEVEL_NO		4
#define LOG_LEVEL_NAME		64

#define LOG_ERROR 			0
#define LOG_WARNING 		1
#define LOG_INFO			2
#define LOG_DEBUG 			3

#define LOG_DEFAULT			3

//最多能打印1024字节的数据
#define LOG_SIZE 			1024

void printfEx(const char * function,unsigned int level,const char *format, ...);

#define DEBUG_LOG

#ifdef DEBUG_LOG

	#ifdef ANDROID
		#define log(x...) ALOGD(x)
	#else
		#define log(level,format...) printfEx(__FUNCTION__,level,format)
		//define log(x...) printf(x)
	#endif
	
#else
	#define log(x...)
#endif

#endif