
#include "log4z.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <iostream>
#include <unistd.h>

using namespace zsummer::log4z;

void test() {

	LOGW("test");
}

int main(int argc, char *argv[])
{

	//设置是否将日志输出到文件中,true:使能输出到文件,false:禁止输出到文件中
//	ILog4zManager::getRef().setLoggerOutFile(LOG4Z_MAIN_LOGGER_ID,false);

	//设置输出文件的路径
	ILog4zManager::getRef().setLoggerPath(LOG4Z_MAIN_LOGGER_ID, "./log2");

	ILog4zManager::getRef().setLoggerName(LOG4Z_MAIN_LOGGER_ID,"zhaokun"); //设置存储文件的名字
/*
存储文件的名字:TestExample_zhaokun_201805292203_028172_000.log
*/

	//设置是否输出文件的名称及行号
	ILog4zManager::getRef().setLoggerFileLine(LOG4Z_MAIN_LOGGER_ID,true);
	//设置输出日志等级>=设置等级的输出
    ILog4zManager::getRef().setLoggerLevel(LOG4Z_MAIN_LOGGER_ID,LOG_LEVEL_TRACE);
//	ILog4zManager::getRef().setLoggerLevel(LOG4Z_MAIN_LOGGER_ID,LOG_LEVEL_INFO);

	//设置是否显示输出到控制台中,true:输出,false:不输出
	ILog4zManager::getRef().setLoggerDisplay(LOG4Z_MAIN_LOGGER_ID,true);

	//设置输出文件的大小,单位是M
	ILog4zManager::getRef().setLoggerLimitsize(LOG4Z_MAIN_LOGGER_ID,1);

	//设置日志是按月进行分文件夹,每月都会有一个文件夹
//	ILog4zManager::getRef().setLoggerMonthdir(LOG4Z_MAIN_LOGGER_ID, true);

	//设置日志能够保留多长时间,前提条件为文件大小已经达到限制的大小
	ILog4zManager::getRef().setLoggerReserveTime(LOG4Z_MAIN_LOGGER_ID,1);
	//启动日志的输出
    ILog4zManager::getRef().start();

//简单输出
#if 0
	LOGT("stream input *** " << "LOGT LOGT LOGT LOGT" << " *** ");
	LOGD("stream input *** " << "LOGD LOGD LOGD LOGD" << " *** ");
	LOGI("stream input *** " << "LOGI LOGI LOGI LOGI" << " *** ");
	LOGW("stream input *** " << "LOGW LOGW LOGW LOGW" << " *** ");
	LOGE("stream input *** " << "LOGE LOGE LOGE LOGE" << " *** ");
	LOGA("stream input *** " << "LOGA LOGA LOGA LOGA" << " *** ");
	LOGF("stream input *** " << "LOGF LOGF LOGF LOGF" << " *** ");
#endif

//c++形式的格式化输出
#if 0
	int id = 10;
	std::string s = "zhaokun";
	LOGF("id = " << id << "," << "s = " << s);
#endif

//C语言形式的格式化输出
#if 0
    LOGFMTT("format input *** %s *** %d ***", "LOGFMTT", 123456);
    LOGFMTD("format input *** %s *** %d ***", "LOGFMTD", 123456);
    LOGFMTI("format input *** %s *** %d ***", "LOGFMTI", 123456);
    LOGFMTW("format input *** %s *** %d ***", "LOGFMTW", 123456);
    LOGFMTE("format input *** %s *** %d ***", "LOGFMTE", 123456);
    LOGFMTA("format input *** %s *** %d ***", "LOGFMTA", 123456);
    LOGFMTF("format input *** %s *** %d ***", "LOGFMTF", 123456);
#endif
    return 0;
}

