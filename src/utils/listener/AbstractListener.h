/*
 **************************************************************************************
 * Filename   : AbstractListner.h
 * Author	  : zhaokun
 * Description: 监听抽象类
 * Date 	  ：2018-01-30
 * Others	  :
 **************************************************************************************
 */

#ifndef ABSTRACTLISTENER_H____
#define ABSTRACTLISTENER_H____

#include <iostream>    
using namespace std;

	//定义抽象的接口类,定义子类要实现的方法
	class AbstractListner {

		public:
			AbstractListner() {};
			virtual ~AbstractListner() {};
			virtual void perform() = 0; //定义纯虚函数
	};

#endif
