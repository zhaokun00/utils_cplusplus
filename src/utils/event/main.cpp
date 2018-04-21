#include "DispatchEvent.h"

//测试C++中enum关键字的使用
void test1() {

	//第一种使用enum方式
//	enum EventType et = EVENT_TIME;

	//第二种使用enum方式
//	EventType et = EVENT_TIME;

	//第三种使用enum方式
	EventType et = EventType::EVENT_TRIGGER;
	
	std::cout << "et = " << et << std::endl;

//测试具有命名空间的enum
#if 0
	ENUM::Type ty = ENUM::Type::TYPE_TRIGGER;

	std::cout << "ty = " << ty << std::endl;
#endif
}

/*
	C++中的std::pair与std::make_pair
	std::pair主要的作用是将两个数据组合成一个数据,两个数据可以是同一类型或者不同类型,例如std::pair<int,float>.
	pair实质上是一个结构体,其主要的两个成员时first和second,这两个变量可以直接使用,初始化一个pair可以使用构造函数,
	也可以使用std::make_pair函数

	注意:使用pair与make_pair要包含头文件#include <utility>
*/
void test2() {

	std::pair<int,std::string> p1(1,"zhao");
	std::pair<int,std::string> p2;

	std::pair<int,std::string> p3;

	p2.first = 2;
	p2.second = "qian";

	p3 = std::make_pair(3,"sun"); //生成std::pair对象

	std::cout << "first = " << p1.first << "," << "second = " << p1.second << std::endl;
	std::cout << "first = " << p2.first << "," << "second = " << p2.second << std::endl;
	std::cout << "first = " << p3.first << "," << "second = " << p3.second << std::endl;
	
}

/*
	map在进行插入的时候是不允许有重复的键值得,如果新插入的键值与原来的键值重复则插入无效,
	可以通过insert的返回值来判断是否成功插入

	insert的函数原型:
	template <class P> 
	std::pair<iterator, bool> insert( P&& value );

	可以通过返回的pair中的第二个bool型变量来判断是否插入成功
*/
//map测试插入重复的数据
void test3() {

	std::map<int ,std::string> map1;

#if 0
	map1.insert(std::make_pair(1,"zhao"));
	map1.insert(std::make_pair(1,"qian")); //，map中插入相同key值是不能插入的

	std::cout << "size = " << map1.size() << std::endl;

	std::map<int,std::string>::iterator it = map1.begin();
	std::cout << "value = " << it->second << std::endl;
#endif

	//第一种形式:直接定义类型
//	std::pair<std::map<int,std::string>::iterator,bool> ret;

	//第二种形式:使用auto
	auto ret = map1.insert(std::make_pair(1,"zhao"));
	if(true == ret.second) {
		std::cout << "第一次插入成功" << std::endl;

		//第一种形式:直接定义类型
	//	std::map<int,std::string>::iterator it = ret.first;

		//第二种形式:使用quto
		auto it = ret.first;
		std::cout << "key = " << it->first << std::endl;
		std::cout << "value = " << it->second << std::endl;
	}
	else {
		std::cout << "第一次插入失败" << std::endl;
	}

	ret = map1.insert(std::make_pair(1,"zhao"));
	if(true == ret.second) {
		std::cout << "第二次插入成功" << std::endl;

		std::map<int,std::string>::iterator it = ret.first;
		std::cout << "key = " << it->first << std::endl;
		std::cout << "value = " << it->second << std::endl;
	}
	else {
		std::cout << "第二次插入失败" << std::endl;
		//即使插入失败,也会返回相对应的迭代器
		std::map<int,std::string>::iterator it = ret.first;
		std::cout << "key = " << it->first << std::endl;
		std::cout << "value = " << it->second << std::endl;
	}

}

typedef char (*PTRFUN)(int);

char testFun(int i) {

	std::cout << "i = " << i << std::endl;
	return 0;
}

void test4_1(PTRFUN fun) {

//	(*fun)(5);
}

class Student {

	public:
		Student() {}
		~Student() {}
		int play(int i) {std::cout << "i = " << std::endl;}
		int i;
};

template<class T>
class Human {
	public:
		typedef int (T::*HandlerFunc)(int i);
	public:
		Human(HandlerFunc func) {}
		~Human() {}

};	
void test4() {

//第一种方式
#if 0
	PTRFUN fun = testFun;
//	fun = testFun;

	(*fun)(2);
#endif

//	test4_1(testFun);

//	test4_1(&Student::play);

	Student s;
	Human<Student> h(&Student::play);

	std::cout << &Student::play << std::endl;
//	std::cout << Student::play << std::endl;
	
}

//

//自定义事件,继承与季磊Event
class TimeEvent : public Event {

	public:
		//在子类构造时对基类进行构造
		TimeEvent() : Event(EventType::EVENT_TIME),
						 m_name("time") {};
		~TimeEvent() {};
		std::string m_name;
};

class TimeEventHandler : public EventHandler {

	public:
		TimeEventHandler() {}
		~TimeEventHandler() {}
		virtual void handlerEvent(Event * event) {

			TimeEvent * ev = (TimeEvent *)event; //将类型强制进行转换

			std::cout << "name = " << ev->m_name << std::endl;
		}
};
		
void test5() {

	TimeEvent * ev = new TimeEvent();

	TimeEventHandler * handler = new TimeEventHandler();

	EventManager * manager = EventManager::create();

	manager->addEventHandler(EventType::EVENT_TIME, handler);

	manager->dispatchEvent(ev);

	EventManager::release();
	
	delete ev;
	ev = NULL;

	delete handler;
	handler = NULL;

	
}

//这种方式,相对于上面的要简单,把成员函数注册了进去,不用在继承EventHandler类了
class UIClass
{
	public:
	    UIClass() {}	    
	    ~UIClass() {}
	    int OnEvent(Event *event) {

			TimeEvent * ev = (TimeEvent *)event; //将类型强制进行转换

			std::cout << "name = " << ev->m_name << std::endl;
	        return 0;
	    }
};

void test6() {

	TimeEvent * ev = new TimeEvent(); //发送的事件,用户还得自己定义
	UIClass ui;
	ClassEventHandler<UIClass> handler(&ui, &UIClass::OnEvent);

	EventManager * manager = EventManager::create();

	manager->addEventHandler(EventType::EVENT_TIME, &handler);

	manager->dispatchEvent(ev);

	EventManager::release();

}

int main() {

//	test1();
//	test2();
//	test3();
//	test4();
//	test5();
	test6();

	return 0;
}

