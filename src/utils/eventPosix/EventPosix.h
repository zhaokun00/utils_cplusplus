/*
 **************************************************************************************
 * Filename   : EventPosix.h
 * Author 	  : zhaokun
 * Description: EventPosix.h 该程序解决的是程序同步问题
 * Date		  ：2018-01-30
 * Others	  :
 **************************************************************************************
 */

/*
	同步/阻塞交互:指发送一个请求,需要等待返回,然后才能够发送下一个请求,有个等待
	异步/非阻塞交互:指发送一个请求,不需要等待返回,随时可以再发送下一个请求,即不需要等待
	区别:一个需要等待,一个不需要等待,在部分情况下,我们的项目开发都会优先选择不需要等待的
	异步交互方式。哪些情况建议使用同步交互呢?比如银行的转账系统,对数据库的保存操作等待都会
	使用交互操作

*/
#ifndef _EVENTPOSIX_H_
#define _EVENTPOSIX_H_

#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <stdint.h>

#define USE_SEMAPHORE

#define RTIP_CLOCK_TYPE_REALTIME

#define RTIP_EVENT_INFINITE 0xffffffff
#define RTIP_EVENT_10_SEC 10000

enum State {
    kUp = 1,
    kDown = 2
};

enum EventTypePosix {
    kEventSignaled = 1,
    kEventError = 2,
    kEventTimeout = 3
};

class EventPosix {

	public:
	    static EventPosix* Create();

	    virtual ~EventPosix();

	    EventTypePosix Wait(uint32_t maxTime);
	    bool Set();
	    bool Reset();

	private:
	    EventPosix();
	    int32_t Construct();

	private:

#ifdef USE_SEMAPHORE
		sem_t m_sem;
#else
	    pthread_cond_t  m_cond;
	    pthread_mutex_t m_mutex;
#endif

	    State m_state;
};

#endif
