/*
 **************************************************************************************
 * Filename   : ThreadPosix.h
 * Author 	  : zhaokun
 * Description: ThreadPosix.h 该文件解决的是创建线程的问题
 * Date		  ：2018-01-30
 * Others	  :
 **************************************************************************************
 */

#ifndef _THREADPOSIX_H_
#define _THREADPOSIX_H_

#include "CriticalSectionPosix.h"
#include "EventPosix.h"

#include <sys/syscall.h>
#include <pthread.h>

#define RTIP_THREAD_RR
#define ThreadObj void*

//class CriticalSectionPosix;
//class EventPosix;

typedef bool (*ThreadRunFunction)(ThreadObj); //定义了一个函数指针

enum ThreadPriority { //定义线程优先级
    kLowPriority = 1,
    kNormalPriority = 2,
    kHighPriority = 3,
    kHighestPriority = 4,
    kRealtimePriority = 5
};

enum {kThreadMaxNameLength = 64};

class ThreadPosix {

public:

	 /*
	  * Function: create thread
	  * Parameters:
	  *	func:function
	  * obj: function parameter
	  * prio: 线程优先级
	  * threadName: 线程的名字
	  *	0 - success
	  *	-1 - failure
	  * note:丢弃数据将来会丢失整个缓冲区的数据,可以根据应用实际情况将来修改代码来适应
	  */
    static ThreadPosix* Create(ThreadRunFunction func = 0, ThreadObj obj = 0,
                             ThreadPriority prio = kNormalPriority, const char* threadName = 0);

    ThreadPosix(ThreadRunFunction func, ThreadObj obj, ThreadPriority prio,
              const char* threadName);

    virtual ~ThreadPosix();

    void SetNotAlive();

	//start thread
    bool Start();
	//stop thread
    bool Stop();

    void Run();
	
private:
    int Construct();

private:

    ThreadRunFunction   m_runFunction;
    ThreadObj           m_obj;

    CriticalSectionPosix* m_pCS;
    bool m_alive;
    bool m_dead;
    ThreadPriority m_prio;
    EventPosix* m_pEvent;

    char m_name[kThreadMaxNameLength];
    bool m_setThreadName;

    pthread_attr_t m_attr;
    pthread_t m_thread;
};

#endif
