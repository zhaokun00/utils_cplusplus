#include <stdio.h>
#include <iostream>
#include <thread>

/*
	g++使用std::thread编译错误问题
	编译报错:what():  Enable multithreading to use std::thread: Operation not permitted Aborted

	命令行解决:
	g++ -Wl,--no-as-needed -std=c++11 -pthread test.cpp

	cmake中解决(添加以下选项):
	SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")
*/
//测试线程的创建
void test1_1() {

	std::cout << "test1_1" << std::endl;
	
}

void test1() {

	std::thread t(test1_1);

	t.join(); //等待线程t完成再往下执行

	/*
		如果没有join语句,会报以下问题:
		terminate called without an active exception
		test1_1Aborted (core dumped)

	*/

}

//测试join语句
void test2_1() {

	//单位是毫秒
	std::chrono::milliseconds dura(1000);

#if 0
	//这样赋值是由问题的
	std::chrono::milliseconds dura;
	dura = 1000;
#endif	
	while(1) {
		std::cout << "test2_1" << std::endl;
		std::this_thread::sleep_for(dura); //睡眠
	}
}

void test2() {

	std::thread t(test2_1);

	t.join(); //如果线程t不结束,不会往下执行

	std::cout << "test2 end" << std::endl;
}

/*
	向子线程中传递的参数时,参数的格式为可变类型
*/
//测试向子线程中传递参数,传递的参数为普通类型,和引用类型
void test3_1(int a,int &b) {

	std::chrono::milliseconds dura(1000);
	std::this_thread::sleep_for(dura);

	std::cout << "test3_1" << std::endl;
	
	a = 20;
	b = 20;

}

void test3() {

	int a = 10;
	int b = 10;
	std::thread t(test3_1,a,std::ref(b)); //传递引用类型时必须用std::ref进行转化

	std::cout << "a = " << a << std::endl; //该处打印并不会等线程t完成才开始往下执行,而是创建完线程后,不管线程是否完成都会一直往下继续执行
	std::cout << "b = " << b << std::endl;
	
	t.join(); //如果线程t不结束,不会往下执行

	std::cout << "a = " << a << std::endl;
	std::cout << "b = " << b << std::endl;
}

//测试向子线程中传递参数,传递的参数为对象和对象的引用类型
class Human {
public:
	Human():id(0) {};
	~Human() {};
	int id;
};

void test4_1(Human h1,Human & h2) {

	h1.id = 10;
	h2.id = 20;
}

void test4() {

	Human h1;
	Human h2;

	std::thread t(test4_1,h1,std::ref(h2));

	t.join();

	std::cout << "h1.id = " << h1.id << std::endl; //打印 0
	std::cout << "h2.id = " << h2.id << std::endl; //打印 20
}

//测试向子线程中传递指针
void test5_1(Human * h1,void * h2) {

	h1->id = 20;
	((Human *)h2)->id = 200;
}

void test5() {

	Human * h1 = new Human();
	Human * h2 = new Human();

	std::thread t(test5_1,h1,(void *)h2);

	t.join();

	std::cout << "h1.id = " << h1->id << std::endl; //打印 20
	std::cout << "h2.id = " << h2->id << std::endl; //打印 200
}

//测试获取线程号
void test6_1() {

	while(1) {
		std::chrono::milliseconds dura(1000);
		std::this_thread::sleep_for(dura);

		std::thread::id id = std::this_thread::get_id();

		std::cout << "thread'd id = " << id << std::endl;

	}
}

void test6() {

	std::thread t(test6_1);
	
	std::cout << "id... = " << t.get_id() << std::endl; //打印t线程的线程号

	std::cout << "main's id" << std::this_thread::get_id() << std::endl; //打印main主线程的线程号
	t.join();

}

//测试返回cpu核数
void test7() {

	std::cout << std::thread::hardware_concurrency() << std::endl;;
}

//测试线程的分类

void test8_1() {

	std::chrono::milliseconds dura(1000);
	

	while(1) {
	//	std::cout << "test8_1" << std::endl;
		printf("test8_1\n");
		std::this_thread::sleep_for(dura);

	}
	
}

void test8() {

	std::chrono::milliseconds dura(1000);
	std::thread t(test8_1);

	std::cout << "test8" << std::endl;

//	t.detach(); //设置线程分类了
//	t.join(); //如果没有等待线程结束,并且线程没有分类,执行时将会报错

	while(1) {

		std::cout << "test8" << std::endl;
		std::this_thread::sleep_for(dura);
	}

}

//线程传入的函数指针,有两种方式
void test9_1() {

	std::cout << "test9_1" << std::endl;
}

void test9() {

//	std::thread t(&test9_1); //可以这样设置

	std::thread t(test9_1); //也可以这样设置

	t.join();
}

//使用成员函数作为线程的执行函数
class Student {

	public:
		void play(int id) {
			std::cout << "student play:" << id << std::endl;
		}
};
		
void test10() {

	Student s;

	std::thread t(&Student::play,s,100);

	t.join();
}

//使用成员函数作为线程的执行函数
class Teacher {

	public:
		Teacher() {

			t = std::thread(&Teacher::play,this,100);
		}
		void play(int id) {
			std::cout << "Teacher play:" << id << std::endl;
		}

	private:
		std::thread t;
};
		
void test11() {

	Teacher t;
	std::chrono::milliseconds dura(1000);

	while(1) {

		std::this_thread::sleep_for(dura);
	}
}

int main() {

//	test1();
//	test2();
//	test3();
//	test4();
//	test5();
//	test6();
//	test7();
//	test8();
//	test9();
//	test10();
	test11();

	return 0;
}

