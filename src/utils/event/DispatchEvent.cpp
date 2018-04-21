#include "DispatchEvent.h"
#if 1
Event::Event(EventType id):m_evntType(id) {

}

Event::~Event() {

}

EventHandler::EventHandler() {}
EventHandler::~EventHandler() {};
void EventHandler::handlerEvent(Event * event) {};

EventManager * EventManager::m_eventManager = new EventManager();

EventManager::EventManager() {

}

EventManager::EventManager(const EventManager & ev) {

}

EventManager::~EventManager() {

}

EventManager& EventManager::operator=(const EventManager &ev) {

    return *this;
}

EventManager * EventManager::create() {

	if(NULL == m_eventManager) {
		return NULL;
	}

	return m_eventManager;
}

void EventManager::release() {

	if(m_eventManager) {
		delete m_eventManager;
		m_eventManager = NULL;
	}
}

int EventManager::addEventHandler(EventType type, EventHandler * handler) {

	if(NULL == handler) {
		return m_error;
	}

	//向map中插入key值与value值
	auto pair = m_handlerMap.insert(std::make_pair(type,HandlerList()));

	auto it = pair.first; //获取迭代器

	if(it == m_handlerMap.end()) { //判断迭代器是否存在
		return m_error;
	}

	//这里应该还有判断在list中是否存在handler,或是使用set存储
	it->second.push_back(handler);
	
	return m_noError;
}

int EventManager::removeEventHandler(EventType type, EventHandler * handler) {

	if(NULL == handler) {
		return m_error;
	}

	auto it = m_handlerMap.find(type);//在map中找出key值,返回的诗迭代器

	if(it == m_handlerMap.end()) { //没有查找到key值
		return m_error;
	}

	auto list = std::find(it->second.begin(),it->second.end(),handler); //在list中查找handler

	if(list != it->second.end()) {
		it->second.erase(list);
	}

	return m_noError;
}

int EventManager::clearEventHander(EventType type) {

	auto it = m_handlerMap.find(type);//在map中找出key值,返回的诗迭代器

	if(it == m_handlerMap.end()) { //没有查找到key值
		return m_error;
	}

	m_handlerMap.erase(type);
}

void EventManager::clear() {

	m_handlerMap.clear();
}

int EventManager::dispatchEvent(Event * event) {

	if(NULL == event) {
		return m_error;
	}
	auto it = m_handlerMap.find(event->m_evntType);

	if(it == m_handlerMap.end()) {
		return m_error;
	}

	auto &list = it->second; //获取的引用
	for(auto item = list.begin();item != list.end();item++) { //获取的迭代器
		if(NULL != (*item)) {
			(*item)->handlerEvent(event);
		}
	}
}

#endif
