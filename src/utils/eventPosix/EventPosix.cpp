#include <errno.h>
#include <pthread.h>
#include <sys/time.h>
#include "EventPosix.h"

const int32_t E6 = 1000000;
const int32_t E9 = 1000 * E6;

#ifdef USE_SEMAPHORE

EventPosix* EventPosix::Create() {

    EventPosix* ptr = new EventPosix;
	
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


EventPosix::EventPosix() : m_state(kDown) {
    
}

int32_t EventPosix::Construct() {

    sem_init(&m_sem, 0, 0);
    return 0;
}

EventPosix::~EventPosix() {

    sem_destroy(&m_sem);
}

bool EventPosix::Reset() {

    m_state = kDown;

    return true;
}

bool EventPosix::Set() {

    m_state = kUp;
    sem_post(&m_sem);
    return true;
}

EventTypePosix EventPosix::Wait(uint32_t timeout) {

    timespec time;
    clock_gettime(CLOCK_REALTIME,&time);

    time.tv_sec += timeout/1000;
    time.tv_nsec += (timeout - (timeout / 1000) * 1000) * E6;;

    if (time.tv_nsec >= E9)
    {
        time.tv_sec++;
        time.tv_nsec -= E9;
    }

    int ret =  sem_timedwait(&m_sem,&time);
	
    switch(ret)
    {
        case 0:
			printf("111111\n");
            return kEventSignaled;
        case -1:
			printf("222222\n");
            return kEventTimeout;
        default:
			printf("333333\n");
            return kEventError;
    }
}

#else

EventPosix* EventPosix::Create() {

    EventPosix* ptr = new EventPosix;
    if (!ptr)
    {
        return NULL;
    }

    const int32_t error = ptr->Construct();
    if (error)
    {
        delete ptr;
        return NULL;
    }

    return ptr;
}


EventPosix::EventPosix() {

    : m_state(kDown)
}

int32_t EventPosix::Construct() {

    int32_t result = pthread_mutex_init(&m_mutex, 0);
    if (result != 0) {
        return -1;
    }

#ifdef RTIP_CLOCK_TYPE_REALTIME
    result = pthread_cond_init(&m_cond, 0);
    if (result != 0)
    {
        return -1;
    }
#else
    pthread_condattr_t condAttr;
    result = pthread_condattr_init(&condAttr);
    if (result != 0)
    {
        return -1;
    }
    result = pthread_condattr_setclock(&condAttr, CLOCK_MONOTONIC);
    if (result != 0)
    {
        return -1;
    }
    result = pthread_cond_init(&m_cond, &condAttr);
    if (result != 0)
    {
        return -1;
    }
    result = pthread_condattr_destroy(&condAttr);
    if (result != 0)
    {
        return -1;
    }
#endif

    return 0;
}

EventPosix::~EventPosix()
{
    pthread_cond_destroy(&m_cond);
    pthread_mutex_destroy(&m_mutex);
}

bool EventPosix::Reset()
{

    if (0 != pthread_mutex_lock(&m_mutex))
    {
        return false;
    }

    m_state = kDown;

    pthread_mutex_unlock(&m_mutex);

    return true;
}

bool EventPosix::Set()
{
    if (0 != pthread_mutex_lock(&m_mutex))
    {
        return false;
    }

    m_state = kUp;

    // Release all waiting threads
    pthread_cond_broadcast(&m_cond);
    pthread_mutex_unlock(&m_mutex);

    return true;
}

EventTypePosix EventPosix::Wait(uint32_t timeout) {

    int32_t retVal = 0;
    if (0 != pthread_mutex_lock(&m_mutex)) {
        return kEventError;
    }

    if (kDown == m_state)
    {
        if (RTIP_EVENT_INFINITE != timeout)
        {
            timespec tEnd;

#ifdef RTIP_CLOCK_TYPE_REALTIME
            clock_gettime(CLOCK_REALTIME, &tEnd);
#else
            clock_gettime(CLOCK_MONOTONIC, &tEnd);
#endif

            tEnd.tv_sec  += timeout / 1000;
            tEnd.tv_nsec += (timeout - (timeout / 1000) * 1000) * E6;

            if (tEnd.tv_nsec >= E9)
            {
                tEnd.tv_sec++;
                tEnd.tv_nsec -= E9;
            }

            retVal = pthread_cond_timedwait(&m_cond, &m_mutex, &tEnd);
        }
        else
        {
            retVal = pthread_cond_wait(&m_cond, &m_mutex);
        }
    }

    m_state = kDown;

    pthread_mutex_unlock(&m_mutex);

    switch(retVal)
    {
        case 0:
            return kEventSignaled;
        case ETIMEDOUT:
            return kEventTimeout;
        default:
            return kEventError;
    }
}

#endif

