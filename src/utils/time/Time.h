/*
 **************************************************************************************
 * Filename   : Time.h
 * Author 	  : zhaokun
 * Description: Time.h 该文件解决的是获取本地时间的问题
 * Date		  ：2018-01-30
 * Others	  :
 **************************************************************************************
 */

#ifndef _TIME_H_
#define _TIME_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <stdarg.h>
	#include <string.h>
	#include <time.h>  
	#include <sys/time.h> 

	#define BASE_YEAR 1900
	
	#define TIME_NOERROR 0
	#define TIME_ERROR -1
	
	#define TIME_BUF_SIZE 256
	#define TIME_BUF_SEC_SIZE  19
	#define TIME_BUF_MSEC_SIZE 23
	/*
	  * Function: getTimeToSec
	  * Description: 获取本地时间,精确到秒数
	  * Parameters:
	  *	pBuf:存放时间的缓冲区,返回格式:YYYY-MM-DD HH:MM:SS
	  * bufLen:缓冲区长度
	  * Return: 0-success -1:failed
	  */
	int getLocalTimeToSec(char *pBuf,int bufLen);

	/*
	  * Function: getTimeToMSec
	  * Description: 获取本地时间,精确到毫秒数
	  * Parameters:
	  *	pBuf:存放时间的缓冲区,返回格式:YYYY-MM-DD HH:MM:SS MS
	  * bufLen:缓冲区长度
	  * Return:0-success -1:failed
	  */
	int getLocalTimeToMsec(char *pBuf,int bufLen);
	
#endif

