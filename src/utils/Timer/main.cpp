#include "Time.h"

//简单测试
void echoFunc(std::string s){
    std::cout << "test : " << s << std::endl;
}

void test1() {

	Timer t1;
    //周期性执行定时任务
    t1.startTimer(1000, std::bind(echoFunc,"hello world!"));

	while(1) {

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

}

//测试传递参数
class Human {

public:
	Human() {}
	~Human()  {}
	void print() { std::cout << "print" << std::endl;}
	void func(int n) { std::cout << "n = " << n << std::endl;}
};

void echoFunc1(Human *h){
   h->print();
}

void test2() {

	Human h;
	Timer t1;
	t1.startTimer(1000, std::bind(echoFunc1,&h));

	while(1) {

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

//测试将成员函数作为执行函数
void test3() {

	Human h;
	Timer t1;
	t1.startTimer(1000, std::bind(&Human::print,&h));

	while(1) {

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

//测试将成员函数作为执行函数,并且传递参数
void test4() {

	Human h;
	Timer t1;
	t1.startTimer(1000, std::bind(&Human::func,&h,10));

	while(1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

//测试关闭定时器
void test5() {

	Timer t1;
    //周期性执行定时任务
    t1.startTimer(1000, std::bind(echoFunc,"hello world!"));

	std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	t1.expire(); //关闭定时器

	while(1) {

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}


//测试同步执行一次
void test6() {

	Human h;
	Timer t1;
	t1.syncWait(2000, std::bind(&Human::func,&h,10));

	std::cout << "syncWait" << std::endl;

	while(1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

}

//测试异步执行一次
void test7() {

	Human h;
	Timer t1;
	t1.asyncWait(2000, std::bind(&Human::func,&h,20));

	std::cout << "asyncWait" << std::endl;

	while(1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

int main() {

//	test1();
	
//	test2();

//	test3();

//	test4();

//	test5();

//	test6();

	test7();	
}
