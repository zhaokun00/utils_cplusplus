/*
 **************************************************************************************
 * Copyright @ 北京猎户星空科技有限公司   1992-2099 All rights reserved
 * Filename   : Object.h
 * Author 	  : zhaokun
 * Description: Object
 * Date		  ：2017-12-07
 * Others	  :
 **************************************************************************************
 */
 
#ifndef OBJECT_FACTORY_H____

#define OBJECT_FACTORY_H____

	#include <map>
	#include <string>

	typedef void* (*NewInstancePt)();
	
	class ReflectFactory {

		public:
	    	static void* CreateObject(const char *className); 

	    	static void RegisterClass(const char *className, NewInstancePt np);
		private:
	    	static std::map<std::string, NewInstancePt> dynCreateMap;
	};

	class Register {

		public:
	   	 Register(const char *className, NewInstancePt np);
	};

//在宏定义中:#表示把其变量转成字符串,##表示把变量连接起来
#define REGISTER_CLASS(class_name) \
	class class_name##Register \
	{ \
	public: \
	    static void* NewInstance() \
	    { \
			return new class_name(); \
	    } \
	private: \
	    static Register reg; \
	}; \
	Register class_name##Register::reg(#class_name, class_name##Register::NewInstance)
	
#endif

