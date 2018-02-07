/*
 **************************************************************************************
 * Filename   : CriticalSectionPosix.h
 * Author 	  : zhaokun
 * Description: CriticalSectionPosix.h,该程序解决的是程序加锁的问题
 * Date		  ：2018-01-30
 * Others	  :
 **************************************************************************************
 */

#ifndef _CRITICALSECTIONPOSIX_H_
#define _CRITICALSECTIONPOSIX_H_

#include <pthread.h>

class CriticalSectionPosix {

	public:
	    static CriticalSectionPosix* Create();

	    virtual ~CriticalSectionPosix();

	    void Enter();
	    void Leave();

	private:
	    CriticalSectionPosix();

	private:
	    pthread_mutex_t m_mutex;
};

class CriticalSectionScoped {

	public:
	    explicit CriticalSectionScoped(CriticalSectionPosix& critsec);

	    explicit CriticalSectionScoped(CriticalSectionPosix* critsec);

	    ~CriticalSectionScoped();

	private:
	    void Leave();
	    CriticalSectionPosix* m_ptrCritSec;
};

#endif
