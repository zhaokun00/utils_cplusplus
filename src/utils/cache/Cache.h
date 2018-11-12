#ifndef CACHE_H_
#define CACHE_H_

#include <stdlib.h>
#include <iostream>
#include <map>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>


template<class T1, class T2>
class Cache {

    public:
        Cache() {
			m_thread = std::thread(&Cache::run, this);
		}
 
        ~Cache(){}
 
        bool get(const T1& key, T2& data) {

            std::unique_lock<std::mutex> lock(m_mutex);

            typename std::map<T1, CacheData>::iterator it = m_cache_map.find(key);

            if(it == m_cache_map.end()) {
                return false;
            }
 
            data = it->second.data;

            return true;
        }
 
        void set(const T1& key, const T2& data, const int& expire_time = -1) {

			std::unique_lock<std::mutex> lock(m_mutex);

            CacheData new_data;
 
            new_data.expire_time = expire_time;
            new_data.update_time = time(NULL);
            new_data.data = data;

            m_cache_map.insert(std::pair<T1, CacheData>(key, new_data));
           
			m_mutexCond.notify_all();
        }
 
        bool remove(const T1& key) {

           std::unique_lock<std::mutex> lock(m_mutex);

            if(m_cache_map.find(key) != m_cache_map.end()) {
                m_cache_map.erase(key);
                
                return true;
            } 
            return false;
        }

    private:

        void expire() {
			std::unique_lock<std::mutex> lock(m_mutex);

            for(typename std::map<T1, CacheData>::iterator it = m_cache_map.begin(); it != m_cache_map.end();) {
				
                if((it->second.expire_time >= 0) && (time(NULL) - it->second.update_time >= it->second.expire_time)) {
                    m_cache_map.erase(it++);
                }
                else {
                    ++it;
                }
            }
        }

       void run() {
            
			for(;;) {

				std::unique_lock<std::mutex> lock(m_mutex);

				if(m_cache_map.empty()) {
					m_mutexCond.wait(lock);
				}

				lock.unlock();
				sleep(1);
				expire();

			}
		}
    private:
		struct CacheData {
			time_t update_time;
			int expire_time;
			T2 data;
		};

        std::map<T1, CacheData> m_cache_map;
        std::mutex m_mutex;
		std::condition_variable m_mutexCond;
		std::thread m_thread;
};
#endif

