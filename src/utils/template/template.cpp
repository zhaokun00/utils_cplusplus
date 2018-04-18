#include <stdio.h>
#include <iostream>
#include <thread>
#include <chrono>


/*
	//以下两种定义是相同的,template在模板定义中关键字class与typename的作用是完全一样的
	template <class identifier> function
	template <typename identifier> function
*/


//测试函数模板
/*
	函数模板声明形式:
	template<typename(或class) T>
	<返回类型><函数名>(参数表) {

	}
*/
template<typename T>
T add(T i,T j) {

	return i + j;
}

void test1() {

	int i = 10;
	int j = 20;
	int z = add(i,j);

	std::cout << "z = " << z << std::endl;
}

//测试类模板
/*
	类模板声明:
	template<class T>
	class 类名 {

	};
*/

template<class E>
class Human {

	public:
		Human();
		~Human();

		E hello(E h);
};

template<class E>
Human<E>::Human() {
	std::cout << "构造函数" << std::endl;
}

template<class E>
Human<E>::~Human() {

	std::cout << "析构函数" << std::endl;
}

template<class E>
E Human<E>::hello(E h) {

	std::cout << "hello" << std::endl;
	return h;
}

void test2() {

	Human<int> h;

	int i = h.hello(100);

	std::cout << "i = " << i << std::endl;
}
int main() {

//	test1();
	
	test2();
//	test3();
//	test4();
//	test5();
//	test6();
//	test7();
//	test8();
//	test9();
//	test10();
	return 0;
}

