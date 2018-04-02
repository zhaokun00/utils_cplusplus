#include "ListenerController.h"

ListenerController::ListenerController()
					: m_count(0) { //C++中需要对成员变量进行初始化操作
		
}

ListenerController::~ListenerController() {

}

void ListenerController::addListener(AbstractListner * listener) {

	m_listListener[m_count++] = listener; //所需要的现象为:没有对m_count进行初始化操作,m_count为系统随机分配的一个变量,因此运行调用时报错
}

int ListenerController::getCount() {

	return m_count;
}

AbstractListner * ListenerController::getListener(int count) {

	return m_listListener[count];
}

