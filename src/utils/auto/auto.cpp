#include <stdio.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <map>

/*
	C++11中引入的auto主要有两种用途:自动类型推断和返回占位符
	auto在C++98中的标识临时变量的语义,由于极少且多余,因此在C++11中已被删除,
	前后两个标准的auto,完全是两个概念
*/

//测试:自动类型推断
/*
	auto自动类型推断,用于初始化表达式中推断出变量的数据类型,通过auto的自动类型推断,可以大大简化我们的编程工作
*/

//测试1:自动帮助推导类型
void test1() {

//	auto a; //错误,没有初始化表达式,无法推断出a的类型
//	auto int a = 10; //错误,auto临时变量的语义在C++11中已不存在,这是旧标准的用法

	auto a = 10;
	auto b = 'A';
	auto c = "zhaokun";

	std::cout << "a = " << a << std::endl;
	std::cout << "b = " << b << std::endl;
	std::cout << "c = " << c << std::endl;
	
}

//测试2:类型冗长
void test2() {

	std::map<int,std::string> map1;

	map1.insert(std::map<int,std::string>::value_type(1,"zhao")); //插入元素
	map1.insert(std::map<int,std::string>::value_type(2,"qian"));

	//普通的定义方式
//	std::map<int,std::string>::iterator it = map1.begin();

	//使用auto类型的定义方式
	auto it = map1.begin();

	std::cout << "key = " << it->first << std::endl;

	std::cout << "value = " << it->second << std::endl;

	auto value = map1.at(2); //参数传入的key值,返回第二个参数的引用

	std::cout << "value = " << value << std::endl;

//查找map中是否存在某个key值
	auto itt = map1.find(3);

	if(itt != map1.end()) {
		std::cout << "map中有该值" << std::endl;
	}
	else {
		std::cout << "map中没有该值" << std::endl;
	}

//根据key值删除map中的元素
	map1.erase(1);

	it = map1.begin();

	std::cout << "key = " << it->first << std::endl;

	std::cout << "value = " << it->second << std::endl;
	
}

//测试3:使用模板技术时,如果某个变量的类型依赖于模板参数,不使用auto将很难确定变量的类型(使用auto后,将由编译器自动进行确定)
template<class T,class E>
void add(T t,E e) {

	auto a = t + e;

	std::cout << "a = " << a << std::endl;
}

void test3() {

	add(1,1.2);
}

//测试4:返回占位符
template<class T,class E>
auto sub(T t,E e) -> decltype(t - e) { //必须加上关键字decltype

	return t - e;
}

void test4() {

	auto a = sub(10,3);

	std::cout << "a = " << a << std::endl;
}

int main() {

//	test1();
//	test2();
//	test3();
	test4();
//	test5();
//	test6();
//	test7();
//	test8();
//	test9();
//	test10();
	return 0;
}

