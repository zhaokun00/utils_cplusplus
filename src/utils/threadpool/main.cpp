#include "threadpool.h"
#include <iostream>

//测试1:简单测试
void fun1(int n) {

	std::cout << "n = " << n << std::endl;
}

void test1() {

    ThreadPool executor(5);

    std::future<void> ff = executor.commit(fun1,100);

    while(1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

//测试2:获取返回值
int fun2(int n) {

	std::cout << "n = " << n << std::endl;

	return n + 1;
}

void test2() {

    ThreadPool executor(5);

    std::future<int> ff = executor.commit(fun2,100);

	int i = ff.get();

	std::cout << "i = " << i << std::endl;

    while(1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

//测试静态成员函数作为变量
class Human {
	
public:
	Human() {}
	~Human() {}
	
	static void print(int n) {std::cout << "print n = " << n << std::endl;}
	static void printf() {std::cout << "无参数"<< std::endl;}

	int func(int n) {
		std::cout << "n = " << n << std::endl;
		return n + 1;
	}
};

//测试普通成员函数作为变量
void test3() {

    ThreadPool executor(5);

    std::future<void> ff = executor.commit(Human::print,100);
	
//	std::future<void> ff = executor.commit(Human::printf);
    while(1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

void test4() {

    ThreadPool executor(5);

	Human h;

    std::future<int> ff = executor.commit(std::bind(&Human::func, &h,100));
	
	int i = ff.get();

	std::cout << "i = " << i << std::endl;
    while(1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

int main() {

//	test1();
//	test2();
//	test3();
	test4();
	return 0;
}

