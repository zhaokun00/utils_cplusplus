#include "DispatchEvent.h"

Event::Event(int id):m_evntType(id) {

}

Event::~Event() {

}

EventManager * EventManager::m_eventManager = new EventManager();

EventManager::EventManager() {

}

EventManager::EventManager(const EventManager & ev) {

}

EventManager& EventManager::operator=(const EventManager & ev) {

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

void EventManager::addEventHandler(EventType type, EventHandler * handler) {

	if(NULL == handler) {
		return;
	}

	//先判断map中有没有该类型
	//再判断
}

void EventManager::removeEventHandler(EventType type, EventHandler * handler) {

}

void EventManager::dispatchEvent(Event * event) {

}
