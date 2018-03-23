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
	/*
		原型:
		int sem_init(sem_t *sem, int pshared, unsigned int value);
		作用:
			是Posix信号量操作中的函数,sem_init()初始化一个定位在sem的匿名信号量
		参数:
			sem:信号量
			pshared:指明信号量是由进程内的线程共享,还是由进程之前共享
				=0:信号量将被进程内的线程共享,并且应该放置在这个进程的所有线程都可见的地址上
				!=0:那么信号量将在进程之间共享
			value:指定信号量的初始值
		返回值:
			0:成功
			-1:错误,并把errno设置为合适的值		
	*/
    sem_init(&m_sem, 0, 0);
    return 0;
}

EventPosix::~EventPosix() {

	/*
		原型:int sem_destroy(sem_t *sem);
		作用:销毁信号量
		sem:要销毁的信号量,只有用sem_init初始化的信号量才能用sem_destory销毁
	*/
    sem_destroy(&m_sem);
}

bool EventPosix::Reset() {

    m_state = kDown;

    return true;
}

bool EventPosix::Set() {

    m_state = kUp;

	/*
		原型:int sem_post(sem_t *sem);
		作用:释放信号量,并让信号量的值加1
		返回值:
			0:成功
			-1:失败
	*/
    sem_post(&m_sem);
    return true;
}

EventTypePosix EventPosix::Wait(uint32_t timeout) {

	int ret = 0;
	
	if(RTIP_EVENT_INFINITE != timeout) { //设置超时时间
		timespec time;
		clock_gettime(CLOCK_REALTIME,&time);
	
		time.tv_sec += timeout/1000;
		time.tv_nsec += (timeout - (timeout / 1000) * 1000) * E6;;
	
		if (time.tv_nsec >= E9)
		{
			time.tv_sec++;
			time.tv_nsec -= E9;
		}
	
		/*
			原型:int sem_wait(sem_t *sem);
			作用:等待信号量,如果信号量的值大于0,将信号量的值减1,立即返回。如果信号量的值为0,则线程阻塞
			参数:
				sem:信号量
			返回值:
				0-成功
				-1:失败
	
	
			原型:int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
			作用:作用与sem_wait相同
			参数:
				sem:信号量
				abs_timeout:指定一个阻塞的时间上限,如果调用因不能立即执行递减而要阻塞,abs_timeout参数指向
				一个指定绝对超时时刻的结构。如果调用时超时时刻已经到点,并且信号量不能立即锁定,那么sem_timedwait()
				将失败于超时-errno设置为ETIMEDOUT。如果操作能被立即执行,那么sem_timedwait()永远不会失败于超时错误,而
				不管abs_timeout 
		*/
	
		ret = sem_timedwait(&m_sem,&time); 
	}
    else { //不设置超时时间,如果获取不到信号,则一直阻塞当前线程
		ret = sem_wait(&m_sem);
	}
	
    switch(ret)
    {
        case 0:
            return kEventSignaled;
        case -1:
            return kEventTimeout;
        default:
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

EventPosix::EventPosix()
    : m_state(kDown)  {
}

int32_t EventPosix::Construct() {

    int32_t result = pthread_mutex_init(&m_mutex, 0);
    if (result != 0) {
        return -1;
    }
	
    result = pthread_cond_init(&m_cond, 0);
    if (result != 0)
    {
        return -1;
    }

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

	/*
		原型:int pthread_cond_broadcast(  pthread_cond_t *cond);
		作用:用来对所有等待参数cond所指定的条件变量的线程解除阻塞
		返回值:
			0:成功
			-1:失败
	*/
    pthread_cond_broadcast(&m_cond);

	/*
		原型:int pthread_cond_signal(  pthread_cond_t *cond);
		作用:用来解除等待参数cond所指定的条件变量的线程阻塞状态,当有多个线程挂起等待条件变量也只唤醒一个线程
	*/
//	pthread_cond_signal(&m_cond);
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

            clock_gettime(CLOCK_REALTIME, &tEnd);

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

