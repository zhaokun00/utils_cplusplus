#include "Time.h"

#if 0

static void _TestTimeToSec() {

	int ret = TIME_NOERROR;

	do {
		/*
	
			time_t相当于long数据类型
			typedef __kernel_time_t 	time_t;
			typedef long		__kernel_time_t;
	
		*/
		time_t t;
	
		/*
			int tm_sec; // 秒 – 取值区间为[0,59] 
			int tm_min; // 分 - 取值区间为[0,59] 
			int tm_hour; // 时 - 取值区间为[0,23] 
			int tm_mday; // 一个月中的日期 - 取值区间为[1,31] 
			int tm_mon; // 月份（从一月开始，0代表一月） - 取值区间为[0,11] 
			int tm_year; // 年份，其值等于实际年份减去1900 
			int tm_wday; // 星期 – 取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推 
			int tm_yday; // 从每年的1月1日开始的天数 – 取值区间为[0,365]，其中0代表1月1日，1代表1月2日，以此类推 
			int tm_isdst; // 夏令时标识符，实行夏令时的时候，tm_isdst为正。不实行夏令时的时候，tm_isdst为0；不了解情况时，tm_isdst()为负。
			long int tm_gmtoff; //指定了日期变更线东面时区中UTC东部时区正秒数或UTC西部时区的负秒数
			const char *tm_zone; //当前时区的名字(与环境变量TZ有关)
		*/
		struct tm *tblock = NULL;
	
		/*
			原型:time_t time(time_t timer);
			作用:函数返回从1900-1-1 0:0:0开始到现在的秒数
	
			用time()函数结合其他函数(localtime、gmtime、asctime、ctime)可以获得当前系统时间或是标准时间
		*/
		t = time(NULL); //也可以这样使用time(&t);
	
	//	printf("seconds is %ld\n",t); //会打印从1900-1-1 0:0:0至现在的秒数
	
		/*
			原型:struct tm *localtime(const time_t *clock);
			作用:把从1900-1-1 0:0:0到当前系统所偏移的秒数时间转换为本地时间,分别存储在时间的各个成员变量中
		*/
		tblock = localtime(&t);

		if(NULL == tblock) {
			ret = TIME_ERROR;
			break;
		}
	
		/*
			原型:char* asctime (const struct tm * timeptr)
			作用:把timerptr指向的tm结构体中存储的时间转换为字符串,返回的字符串格式为:
			Www Mmm dd hh:mm:ss yyyy。其中Www为星期；Mmm为月份；dd为日；hh为时；mm为分；ss为秒；yyyy为年份
		*/
		printf("local time is: %s",asctime(tblock));
	
		//打印年、月、
		printf("year = %d,month = %d,day = %d,hour = %d,minute = %d,second = %d\n",tblock->tm_year + BASE_YEAR,tblock->tm_mon,tblock->tm_mday,tblock->tm_hour,tblock->tm_min,tblock->tm_sec);
	
		char buf[TIME_BUF_SIZE] = {0};
	
		/*
			int snprintf(char *str, size_t size, const char *format, ...)
			作用:格式化字符串,输出结果将按照format格式存放到str中
	
			int sscanf(const char *str, const char *format, ...)
			作用:将参数str的字符串根据参数format字符串来转换并格式化为数据,转换后的结果存在于相对应的参数内
			
		*/
	
		snprintf(buf,sizeof(buf),"%d-%d-%d %d:%d:%d",tblock->tm_year + BASE_YEAR,tblock->tm_mon,tblock->tm_mday,tblock->tm_hour,tblock->tm_min,tblock->tm_sec);
	
		printf("buf = %s\n",buf);

		
	} while(0);
	
}

static void _TesttimeToUsec() {

	int ret = TIME_NOERROR;

	do {
		/*
			struct timeval {
				long tv_sec;  //秒
				long tv_usec; //微妙
			};	
		
			struct timezone {
				int tz_minuteswest; //和Greenwich时间差了多少分钟
				int tz_dsttime; 	//日光节约时间的状态
			};
		*/
		
		struct timeval tv;
		struct timezone tz;
		
		/*
			int gettimeofday(struct timeval *tv, struct timezone *tz);
			作用:会把当前的时间由tv所指向的结构返回,当地时区的信息则放在tz所指的结构中,可用NULL替代
			返回值:成功返回0,失败返回-1,错误代码存于errno中
		*/
		ret = gettimeofday(&tv,&tz);

		if(TIME_NOERROR != ret) {
			ret = TIME_ERROR;
			break;
		}

		printf("sec = %ld,usec = %ld\n",tv.tv_sec,tv.tv_usec);

		struct tm *tblock = NULL;

		tblock = localtime(&tv.tv_sec);
		
		if(NULL == tblock) {
			ret = TIME_ERROR;
			break;
		}

		printf("year = %d,month = %d,day = %d,hour = %d,minute = %d,second = %d\n",tblock->tm_year + BASE_YEAR,tblock->tm_mon,tblock->tm_mday,tblock->tm_hour,tblock->tm_min,tblock->tm_sec);

	} while(0);

}

static void _TestTimeToNsec() {

	int ret = TIME_NOERROR;

	do {
		/*
			struct timespec {  
	            time_t tv_sec; //秒
	            long tv_nsec;  //纳秒
        	};
		*/
		struct timespec ts;

		/*

			1.在编译链接时需加上 -lrt ;因为在librt中实现了clock_gettime函数  
			2.int clock_gettime(clockid_t clk_id, struct timespec *tp) 
				
			参数说明：  
				clockid_t clk_id 用于指定计时时钟的类型，有以下4种：  
				CLOCK_REALTIME:系统实时时间,随系统实时时间改变而改变,即从UTC1970-1-1 0:0:0开始计时,中间时刻如果系统时间被用户该成其他,则对应的时间相应改变  
				CLOCK_MONOTONIC:从系统启动这一刻起开始计时,不受系统时间被用户改变的影响	
				CLOCK_PROCESS_CPUTIME_ID:本进程到当前代码系统CPU花费的时间  
				CLOCK_THREAD_CPUTIME_ID:本线程到当前代码系统CPU花费的时间  
				struct timespec *tp用来存储当前的时间，其结构如下：

		*/
		ret = clock_gettime(CLOCK_REALTIME,&ts);

		if(TIME_NOERROR != ret) {
			ret = TIME_ERROR;
			break;
		}

		printf("sec = %ld,nsec = %ld\n",ts.tv_sec,ts.tv_nsec);
	} while(0);
	
}

#endif 

int getLocalTimeToSec(char *pBuf,int bufLen) {

	int ret = TIME_NOERROR;
	time_t t;
	struct tm *tblock = NULL;
	
	do {

		if((pBuf == NULL) || (bufLen < bufLen)) {
			ret = TIME_ERROR;
			break;
		}

		memset(pBuf,0,bufLen);

		t = time(NULL);		
		tblock = localtime(&t);
	
		if(NULL == tblock) {
			ret = TIME_ERROR;
			break;
		}
	
		snprintf(pBuf,bufLen,"%d-%02d-%02d %02d:%02d:%02d",tblock->tm_year + BASE_YEAR,tblock->tm_mon,tblock->tm_mday,tblock->tm_hour,tblock->tm_min,tblock->tm_sec);

	} while(0);
	
	return ret;
}

int getLocalTimeToMsec(char *pBuf,int bufLen) {

	int ret = TIME_NOERROR;
	struct timeval tv;
	struct timezone tz;
	struct tm *tblock = NULL;
	
	do {

		if((pBuf == NULL) || (bufLen < bufLen)) {
			ret = TIME_ERROR;
			break;
		}
				
		ret = gettimeofday(&tv,&tz);

		if(TIME_NOERROR != ret) {
			ret = TIME_ERROR;
			break;
		}

		tblock = localtime(&tv.tv_sec);
		
		if(NULL == tblock) {
			ret = TIME_ERROR;
			break;
		}

		snprintf(pBuf,bufLen,"%d-%02d-%02d %02d:%02d:%02d %03ld",tblock->tm_year + BASE_YEAR,tblock->tm_mon,tblock->tm_mday,tblock->tm_hour,tblock->tm_min,tblock->tm_sec,tv.tv_usec/1000);
	} while(0);

	return ret;
}

