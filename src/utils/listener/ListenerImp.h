/*
 **************************************************************************************
 * Filename   : ListenerImp.h
 * Author	  : zhaokun
 * Description: 抽象监听类的实现
 * Date 	  ：2018-01-30
 * Others	  :
 **************************************************************************************
 */

#ifndef LISTENERIMP_H____
#define LISTENERIMP_H____
#include "AbstractListener.h"
#include <stdio.h>

	//定义实现的子类,子类要继承接口,实现父类中的方法
	class ListenerImp : public AbstractListner {

		public:
			ListenerImp();
			virtual ~ ListenerImp();
			virtual void perform();
	};
#endif
