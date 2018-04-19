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
void test3() {

	std::map<int ,std::string> map1;

#if 0
	map1.insert(std::make_pair(1,"zhao"));
	map1.insert(std::make_pair(1,"qian")); //，map中插入相同key值是不能插入的

	std::cout << "size = " << map1.size() << std::endl;

	std::map<int,std::string>::iterator it = map1.begin();
	std::cout << "value = " << it->second << std::endl;
#endif
	
//	auto pair = map1.insert(std::make_pair(1,"zhao"));

//	int it = pair.first;
//	std::cout << "first = " << it << std::endl;
//	std::cout << "second = " << pair.second << std::endl;
	
}

int main() {

//	test1();
//	test2();

	test3();
	return 0;
}

