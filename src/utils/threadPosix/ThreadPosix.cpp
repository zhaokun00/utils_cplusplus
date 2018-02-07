#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/prctl.h>

#include "ThreadPosix.h"

extern "C"
{
    static void* StartThread(void* lpParameter) {
        static_cast<ThreadPosix*>(lpParameter)->Run();

        return 0;
    }
}

ThreadPosix* ThreadPosix::Create(ThreadRunFunction func, ThreadObj obj,
                                 ThreadPriority prio, const char* threadName) {

	ThreadPosix* ptr = new ThreadPosix(func, obj, prio, threadName); //调用构造函数
    if (!ptr) {
        return NULL;
    }

    const int32_t error = ptr->Construct();
    if (error) {
        delete ptr;

        return NULL;
    }

    return ptr;
}

ThreadPosix::ThreadPosix(ThreadRunFunction func, ThreadObj obj,
                         ThreadPriority prio, const char* threadName)
    : m_runFunction(func),
      m_obj(obj),
      m_pCS(CriticalSectionPosix::Create()),
      m_alive(false),
      m_dead(true),
      m_prio(prio),
      m_pEvent(EventPosix::Create()),
      m_name(),
      m_setThreadName(false),
      m_attr(),
      m_thread(0) {    

    if (threadName != NULL) {
        m_setThreadName = true;
        strncpy(m_name, threadName, kThreadMaxNameLength);
        m_name[kThreadMaxNameLength - 1] = '\0';
    }
}

int ThreadPosix::Construct() {

	int32_t result = 0;

    result = pthread_attr_init(&m_attr); //线程初始化
    if (result != 0)
    {
        return -1;
    }

    return 0;
}

ThreadPosix::~ThreadPosix() {

    pthread_attr_destroy(&m_attr);
    delete m_pEvent;
    delete m_pCS;
}

bool ThreadPosix::Start() {

    if (!m_runFunction) {
        return false;
    }

    int32_t result = pthread_attr_setdetachstate(&m_attr, PTHREAD_CREATE_DETACHED);
    
    result |= pthread_attr_setstacksize(&m_attr, 1024*1024); //设置栈信息

#ifdef RTIP_THREAD_RR
    const int32_t policy = SCHED_RR;
#else
    const int32_t policy = SCHED_FIFO;
#endif

    m_pEvent->Reset();

    result |= pthread_create(&m_thread, &m_attr, &StartThread, this); //创建线程
    if (result != 0)
    {
        return false;
    }

    // Wait up to 10 seconds for the OS to call the callback function. Prevents
    // race condition if Stop() is called too quickly after start.
    if (kEventSignaled != m_pEvent->Wait(RTIP_EVENT_10_SEC)) { //将来会等待线程中释放信号量才能往下继续执行
        // Timed out. Something went wrong.
        m_runFunction = NULL;
        return false;
    }

    sched_param param;

    const int32_t minPrio = sched_get_priority_min(policy);
    const int32_t maxPrio = sched_get_priority_max(policy);
    if ((minPrio == EINVAL) || (maxPrio == EINVAL)) {
        return false;
    }

    switch (m_prio)
    {
    case kLowPriority:
        param.sched_priority = minPrio + 1;
        break;
    case kNormalPriority:
        param.sched_priority = (minPrio + maxPrio) / 2;
        break;
    case kHighPriority:
        param.sched_priority = maxPrio - 3;
        break;
    case kHighestPriority:
        param.sched_priority = maxPrio - 2;
        break;
    case kRealtimePriority:
        param.sched_priority = maxPrio - 1;
        break;
    }
    result = pthread_setschedparam(m_thread, policy, &param);
	
    if (result == EINVAL) {
        return false;
    }

    return true;
}

void ThreadPosix::SetNotAlive() {

    CriticalSectionScoped cs(m_pCS);
    m_alive = false;
}

bool ThreadPosix::Stop() {

    bool dead = false;
    {
        CriticalSectionScoped cs(m_pCS);
        m_alive = false;
        dead = m_dead;
    }

    for (uint32_t i = 0; i < 1000 && !dead; i++) {
   
        timespec t;
        t.tv_sec = 0;
        t.tv_nsec = 10*1000*1000;
        nanosleep(&t, NULL);
        {
            CriticalSectionScoped cs(m_pCS);
            dead = m_dead;
        }
    }

    if (dead) {
    
        return true;
    }
    else {
    
        return false;
    }
}

void ThreadPosix::Run() {

    {
        CriticalSectionScoped cs(m_pCS);
        m_alive = true;
        m_dead  = false;
    }

    m_pEvent->Set(); //释放信号量

    if (m_setThreadName) {
        prctl(PR_SET_NAME, m_name); //设置线程名字
    }
    else {
    
    }

    bool alive = true;
    do
    {
        if (m_runFunction)
        {
            if (!m_runFunction(m_obj)) //执行运行函数,当运行函数返回false时将会跳出while循环
            {
                alive = false;
            }
        }
        else
        {
            alive = false;
        }
        {
            CriticalSectionScoped cs(m_pCS);
            if (!alive)
            {
              m_alive = false;
            }
            alive = m_alive;
        }
    }
    while (alive); //会在这里循环执行,当调动Stop函数时线程停止运行

    if (m_setThreadName) {

    }

    {
        CriticalSectionScoped cs(m_pCS);
        m_dead = true;
    }
}
