#ifndef TIME_H_
#define TIME_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>  
#include <sys/time.h> 

#include<iostream>
#include<functional>
#include<chrono>
#include<thread>
#include<atomic>
#include<memory>
#include<mutex>
#include<condition_variable>

class Timer{
	
public:
    Timer() :m_expired(true), m_tryTtoExpire(false) {
		
    }
 
    Timer(const Timer& t){
        m_expired = t.m_expired.load();
        m_tryTtoExpire = t.m_tryTtoExpire.load();
    }
	
    ~Timer(){
        expire();
    }
 
	//启动定时器
    void startTimer(int interval, std::function<void()> task){

        if (false == m_expired){
            return;
        }		
        m_expired = false;
		
        std::thread([this, interval, task](){
            while (!m_tryTtoExpire){				
                std::this_thread::sleep_for(std::chrono::milliseconds(interval));
                task();
            }
			
            {
                std::lock_guard<std::mutex> locker(m_mutex);
                m_expired = true;
                m_expiredcond.notify_one();
            }
			
        }).detach();
    }
 
	//关闭定时器
    void expire(){
        if (m_expired) {
            return;
        }
 
        if ( m_tryTtoExpire){
            return;
        }
        m_tryTtoExpire = true;
		
        {
            std::unique_lock<std::mutex> locker(m_mutex);
            m_expiredcond.wait(locker, [this]{return m_expired == true; });
            if (m_expired == true) {
                m_tryTtoExpire = false;
            }
        }
    }
    
	//同步执行一次 
    template<typename callable, class... arguments>
    void syncWait(int after, callable&& f, arguments&&... args){
 
        std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));
        std::this_thread::sleep_for(std::chrono::milliseconds(after));
        task();
    }
	
	//异步执行一次
    template<typename callable, class... arguments>
    void asyncWait(int after, callable&& f, arguments&&... args){
        std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));
 
        std::thread([after, task](){
            std::this_thread::sleep_for(std::chrono::milliseconds(after));
            task();
        }).detach();
    }
     
private:
    std::atomic<bool> m_expired;
    std::atomic<bool> m_tryTtoExpire;
    std::mutex m_mutex;
    std::condition_variable m_expiredcond;
};
	
#endif

