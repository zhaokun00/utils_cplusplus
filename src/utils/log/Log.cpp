#include "Log.h"

LogUtils * LogUtils::logUtils = new LogUtils();

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

LogUtils::LogUtils() {

}

LogUtils::LogUtils(const LogUtils &) {

}

LogUtils& LogUtils::operator=(const LogUtils &LogUtils) {

    return *this;
}

LogUtils * LogUtils::getInstance() {

    if(logUtils != NULL) {
        return logUtils;
    }

    return NULL;
}

void LogUtils::init(char * programName) {

		google::InitGoogleLogging(programName);
	
		//设置是否将所有日志输出到stderr,而非文件,如果该项定义为true,则之间日志信息输出到屏幕上而不输出到日志中
//		FLAGS_logtostderr = true;

		//限制输出到stderr的部分信息,只有高于该级别的日志信息才会输出到屏幕中或者文件中
		FLAGS_minloglevel = google::WARNING;
	
		//是否同时将日志输出到文件和stderr,该配置可以同时将日志信息输出到屏幕上和日志中,如果FLAGS_logtostderr = true;该项将不起作用
		FLAGS_alsologtostderr = true;

		//是否将不同级别的日志文件分割开来,该处为自己修改的源码,原始的日志文件低优先级的日志会包含高优先级的日志
		FLAGS_servity_single_log = true;

		//设置记录到标准输出的颜色消息,如果终端支持
		FLAGS_colorlogtostderr = true;

		//设置日志前缀是否应该添加到每行日志的输出,即日志格式是否输出,true为输出,false为不输出,日志格式自己也修改了源码
//		FLAGS_log_prefix = false;

		//缓冲日志输出，默认为30秒，此处改为0立即输出
		FLAGS_logbufsecs = 0;

		//设置最大日志大小,单位MB,当文件达到最大值时会重新再次创建一个新的文件
		FLAGS_max_log_size = 1;

		//当磁盘被写满时，停止日志输出
		FLAGS_stop_logging_if_full_disk = true; 

		//设置日志文件的输出目录,如果目录不存在则将不输出日志到文件
//		FLAGS_log_dir = "/data/10.100.37.17/home/zhaok/test/utils_cplusplus/src/utils/log/";

		//以下可以覆盖前面的日志路径
#if 1
		//设置google::FATAL 级别的日志存储路径和文件名前缀
		google::SetLogDestination(google::GLOG_FATAL, "./log/fatal_");
		//设置 google::ERROR 级别的日志存储路径和文件名前缀
		google::SetLogDestination(google::GLOG_ERROR, "./log/error_");
		//设置 google::WARNING 级别的日志存储路径和文件名前缀
		google::SetLogDestination(google::GLOG_WARNING, "./log/warning_");
		//设置 google::INFO 级别的日志
		google::SetLogDestination(google::GLOG_INFO, "./log/info_");
#endif

		//设置文件扩展名,可以在文件名的后面添加该信息,可以用于区分不同的文件
//		google::SetLogFilenameExtension("_extension_");

		//设置捕捉 core dump
		google::InstallFailureSignalHandler();
		
}

void LogUtils::close() {

	google::ShutdownGoogleLogging();

	if(logUtils) {
		delete logUtils;
		logUtils = NULL;
	}
}



