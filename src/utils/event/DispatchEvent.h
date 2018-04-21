#ifndef DISPATCH_EVENT_H____
#define DISPATCH_EVENT_H____

	#include <iostream>
	#include <thread> 								//线程的头文件
	#include <map> 									//map的头文件
	#include <list> 								//list的头文件
	#include <mutex> 								//互斥锁的头文件
	#include <condition_variable>					//条件变量的头文件
	#include <chrono>									
	#include <memory> 								//智能指针的头文件
	#include <queue> 								//队列的头文件
	#include <string>								//字符串的头文件
	#include <algorithm>
	#include <string>
	enum EventType {

		EVENT_TIME = 0,
		EVENT_TRIGGER = 1,
	};

//http://www.cnblogs.com/kevonyang/p/5965825.html
#if 0
	namespace ENUM {

		enum Type {
			TYPE_TIME = 0,
			TYPE_TRIGGER = 1,
		};
	}
#endif
#if 1

	//定义事件,不同事件继承与该事件
	class Event {
		public:
			Event(EventType id);
			~Event();
			
			EventType m_evntType; //用于区分事件类型
	};

	
	class EventHandler {

		public:
			EventHandler();
			virtual ~EventHandler();
			virtual void handlerEvent(Event * event);
	};

	class EventManager {

		public:
			typedef std::list<EventHandler *> HandlerList; //定义了一个list列表
			typedef std::map<int,HandlerList> HandlerMap; //map中存放键值:事件类型,列表:存放的handler

		public:
			
			//设置成单例模式
			static EventManager * create();
			
			static void release();
			
			int addEventHandler(EventType type,EventHandler * handler);
			
			int removeEventHandler(EventType type,EventHandler * handler);

			int clearEventHander(EventType type);

			void clear();
			
			int dispatchEvent(Event * event);
			
		private:

			static const int m_error = -1;
			static const int m_noError = 0;
			static EventManager * m_eventManager;
			HandlerMap m_handlerMap;
			
			EventManager();
			EventManager(const EventManager &);
			~EventManager();
			EventManager & operator=(const EventManager &);


	};

	template<class T>
	class ClassEventHandler : public EventHandler {

		public:
			typedef int (T::*HandlerFunc)(Event * ev);
		public:
			ClassEventHandler(T *ptr, HandlerFunc func) :
										  m_ptr(ptr),
										  m_func(func) {
			}

			virtual void handlerEvent(Event * event) {
				if(m_ptr && m_func) {

					(m_ptr->*m_func)(event);
				}
			}
		private:
			T * m_ptr;
			HandlerFunc m_func;
	};
#endif
#endif

