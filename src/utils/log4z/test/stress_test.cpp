
#include "../log4z.h"
#include <stdio.h>
#include <string.h>
#include <signal.h>
#if WIN32
#include <Windows.h>
#include <process.h>
#else
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#endif
using namespace zsummer::log4z;

//! cross platform function:
void sleepMillisecond(unsigned int ms);
bool createThread(void(*run)());


//! multi logger id
LoggerId logid_mysql;
LoggerId logid_network;
LoggerId logid_moniter;



#define  LOG_CONTENT "char:" <<'c'\
<< ", unsigned char:" << (unsigned char) 'c'\
<< ", short:" << (short) -1\
<< ", unsigned short:" << (unsigned short) -1\
<< ", int:" << (int) -1\
<< ", unsigned int:" << (unsigned int) -1\
<< ", long:" << (long) -1\
<< ", unsigned long:" << (unsigned long) -1\
<< ", long long:" << (long long) -1\
<< ", unsigned long long:" << (unsigned long long) -1\
<< ", float:" << (float) -1.234567\
<< ", double:" << (double) -2.34566\
<< ", std::string:" << std::string("fffff")\
<< ", void *:" << ( int *) 32423324\
<< ", const void*:" << (const int *) 32423324\
<< ", constant:" << 1000 \
<< ", constant:" << 100.12345678\
<< ", bool:" << (bool) true
//#define LOG_CONTENT "aaaaaaaaaaaa"


#define  LOG_CONTENT2 "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"

#define  LOG_CONTENT_WINFMT "char:%c, unsigned char:%u, short:%d, unsigned short:%u, int:%d, unsigned int:%u, long:%d, unsigned long:%u, long long:%I64d, unsigned long long:%I64u, \
                                            float:%f, double:%lf, string:%s, void*:%x, const void*:%x, constant:%d, constant:%lf, bool:%d", \
                                            'c', (unsigned char) 'c', (short)-1, (unsigned short)-1, \
                                            (int)-1, (unsigned int)-1, (long)-1, (unsigned long)-1, (long long)-1, (unsigned long long) - 1, \
                                            (float)-1.234567, (double)-2.34566, "fffff", \
                                            32423324, 234, 1000, 100.12345678, true

#define  LOG_CONTENT_LINUXFMT "char:%c, unsigned char:%u, short:%d, unsigned short:%u, int:%d, unsigned int:%u, long long:%lld, unsigned long long:%llu, \
                                            float:%f, double:%lf, string:%s, void*:%x, const void*:%x, constant:%d, constant:%lf, bool:%d", \
                                            'c', (unsigned char) 'c', (short)-1, (unsigned short)-1, \
                                            (int)-1, (unsigned int)-1, (long long)-1, (unsigned long long) - 1, \
                                            (float)-1.234567, (double)-2.34566, "fffff", \
                                            32423324, 234, 1000, 100.12345678, true


//! process quit.
bool g_quit;

#define STREES_SWITCH 2 // 1 stream, 2 stream simple, 3 windows format, 4 linux format

void multiThreadFunc()
{
    unsigned long long count = 0;
    while(g_quit)
    {
        count++;
#if (STREES_SWITCH == 1)
        {
            LOG_ERROR(logid_mysql, LOG_CONTENT);
            LOG_FATAL(logid_network, LOG_CONTENT);
            LOG_WARN(logid_moniter, LOG_CONTENT);
        }
#elif (STREES_SWITCH == 2)
        {
            LOG_ERROR(logid_mysql,   Log4zString(LOG_CONTENT2, sizeof(LOG_CONTENT2)));
            LOG_FATAL(logid_network, Log4zString(LOG_CONTENT2, sizeof(LOG_CONTENT2)));
            LOG_WARN(logid_moniter, LOG_CONTENT2);
        }
#elif (STREES_SWITCH == 3)
        {
            LOGFMT_ERROR(logid_mysql, LOG_CONTENT_WINFMT);
            LOGFMT_FATAL(logid_network, LOG_CONTENT_WINFMT);
            LOGFMT_WARN(logid_moniter, LOG_CONTENT_WINFMT);
        }
#elif (STREES_SWITCH == 4)
        {
            LOGFMT_ERROR(logid_mysql, LOG_CONTENT_LINUXFMT);
            LOGFMT_FATAL(logid_network, LOG_CONTENT_LINUXFMT);
            LOGFMT_WARN(logid_moniter, LOG_CONTENT_LINUXFMT);
        }
#endif




    }
    LOGA("thread quit ... ");
}


void signalFunc(int id)
{
    g_quit = false;
#ifdef WIN32
    signal(id, &signalFunc);
#endif
}



int main(int argc, char *argv[])
{
    g_quit = true;
    signal(SIGINT, &signalFunc);

    //! ---------
    logid_mysql = ILog4zManager::getRef().createLogger("mysql" );
    logid_network = ILog4zManager::getRef().createLogger("network" );
    logid_moniter = ILog4zManager::getRef().createLogger("moniter" );
    ILog4zManager::getRef().config("config.cfg");
    //! ---------
    ILog4zManager::getRef().setLoggerDisplay(logid_mysql, false);
    ILog4zManager::getRef().setLoggerDisplay(logid_network, false);
    ILog4zManager::getRef().setLoggerDisplay(logid_moniter, false);
    //ILog4zManager::getRef().setLoggerOutFile(logid_mysql, false);
    //ILog4zManager::getRef().setLoggerOutFile(logid_network, false);
    //ILog4zManager::getRef().setLoggerOutFile(logid_moniter, false);
	ILog4zManager::getRef().setLoggerReserveTime(logid_mysql, 3*20);
	ILog4zManager::getRef().setLoggerReserveTime(logid_network, 2*20);
	ILog4zManager::getRef().setLoggerReserveTime(logid_moniter, 20);

    //! ---------
    ILog4zManager::getRef().start();




    //! ---------
    for (int i=0; i<1; i++)
    {
        createThread(&multiThreadFunc);
    }
   

    //! ---------
    unsigned long long lastCount = 0;
    unsigned long long lastData = 0;
    
    while(g_quit)
    { 
        unsigned long long speedCount = ILog4zManager::getRef().getStatusTotalWriteCount() - lastCount;
        lastCount += speedCount;
        unsigned long long speedData = ILog4zManager::getRef().getStatusTotalWriteBytes() - lastData;
        lastData += speedData;
        LOGI("Stress Status:  Write Speed: " << speedCount/5 
            << " n/s, Speed: " << speedData/1024/5 
            << " KB/s, Waiting: " << ILog4zManager::getRef().getStatusTotalPushQueue() - ILog4zManager::getRef().getStatusTotalPopQueue());
        sleepMillisecond(5000);
    }

    //! wait all thread quit ...
    //! skip wait code ...
    LOGA("main quit ... ");
    return 0;
}


//////////////////////////////////////////////////////////////////////////
void sleepMillisecond(unsigned int ms)
{
#ifdef WIN32
    ::Sleep(ms);
#else
    timespec cur;
    cur.tv_sec = ms/1000;
    cur.tv_nsec = (ms%1000)*1000*1000;
    timespec remaining;
    while (nanosleep(&cur, &remaining) == -1 && errno == EINTR)
    {
        memcpy(&cur,&remaining, sizeof(timespec));
    }
#endif
}



//
#ifdef WIN32
 static unsigned int WINAPI  threadProc(LPVOID lpParam)
{
    ((void(*)())(lpParam))();
    _endthreadex(0);
    return 0;
}
#else
static void * threadProc(void * pParam)
{
    ((void(*)())(pParam))();
    return NULL;
}
#endif

bool createThread(void(*run)())
{
#ifdef WIN32
    unsigned long long ret = _beginthreadex(NULL, 0, threadProc,(void*) run, 0, NULL);

    if (ret == -1 || ret == 1  || ret == 0)
    {
        return false;
    }
#else
    pthread_t ptid = 0;
    int ret = pthread_create(&ptid, NULL, threadProc, (void*)run);
    if (ret != 0)
    {
        return false;
    }
#endif
    return true;
}

