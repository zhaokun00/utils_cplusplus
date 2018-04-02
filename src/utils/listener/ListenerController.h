/*
 **************************************************************************************
 * Filename   : ListenerImp.h
 * Author	  : zhaokun
 * Description: 抽象监听类的实现
 * Date 	  ：2018-01-30
 * Others	  :
 **************************************************************************************
 */

#ifndef LISTENER_CONTROLLER_H____
#define LISTENER_CONTROLLER_H____

#include "AbstractListener.h"
#include<iostream>
#include<stdio.h>

	//定义监听器的控制类,可用通过该类实现添加监听器、移除监听器等,类似于管理者,客户端只需要拿到该实例即可
	class ListenerController {
		public:
			ListenerController();
			~ListenerController();
			//要实现多态必须使用指针的形式(AbstractListner *),而不能使用对象形式(AbstractListner),对象形式不能实现多态
			void addListener(AbstractListner * listener);
			int getCount();
			AbstractListner * getListener(int count);
			
		private:
			AbstractListner * m_listListener[10];
			int m_count;
	};	

#endif

