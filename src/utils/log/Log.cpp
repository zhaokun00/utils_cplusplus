#include "Log.h"

static char LOG_TAG[LOG_LEVEL_NO][LOG_LEVEL_NAME] = {
	"E",
	"W",
	"I",
	"D"
};

void printfEx(const char * function,unsigned int level,const char *format, ...) {

	int ret = 0;
	unsigned int bufLen = 0;
	unsigned int timeLen = 0;
	unsigned int levelLen = 0;
	unsigned int dataLen = LOG_SIZE;
	unsigned int funcLen = 0;
	unsigned int offsetLen = 0;
	char timeBuf[24] = {0}; //存放时间,必须预留一个空间存储0,若不预留则会截取最后一个空间为0
	struct timeval tv;
	struct timezone tz;
	struct tm *tblock = NULL;	
	
	do {

		if((level < LOG_ERROR) || (LOG_ERROR > LOG_DEBUG)) { //检查日志级别
			break;
		}

		if(level > LOG_DEFAULT) { //检查是否要打印日志,只有小于LOG_DEFAULT才会打印
			break;
		}

		ret = gettimeofday(&tv,&tz);

		if(0 != ret) {
			break;
		}

		tblock = localtime(&tv.tv_sec);
		
		if(NULL == tblock) {
			break;
		}

		snprintf(timeBuf,sizeof(timeBuf),"%04d-%02d-%02d %02d:%02d:%02d %03ld",tblock->tm_year + 1900,tblock->tm_mon,tblock->tm_mday,tblock->tm_hour,tblock->tm_min,tblock->tm_sec,tv.tv_usec/1000);

		timeLen = strlen(timeBuf);
		levelLen = strlen(LOG_TAG[level]);
		funcLen = strlen(function);
		
		va_list args;
		va_start(args, format); //指向可变的参数
		
		bufLen = timeLen + levelLen + funcLen + dataLen;
		
		char *pBuf = NULL;		
		pBuf = (char *)malloc(bufLen);

		if(NULL == pBuf) {
			break;
		}
		
		memset(pBuf,0,bufLen);
		
		offsetLen = timeLen + levelLen + 2 + 1;

		snprintf(pBuf,offsetLen,"%s %s ",timeBuf,LOG_TAG[level]);
	
		vsnprintf(pBuf + strlen(pBuf), dataLen, format, args); //将可变参数放入缓冲区中
		
		va_end(args);

		printf("%s\n",pBuf); //将缓冲区数据输出
		
		free(pBuf);
		pBuf = NULL;
	} while(0);
	
}


