#include <stdio.h>
#include <iostream>
#include <deque>
#include <chrono>

//测试C++中接口的定义
class Human {

public:
	virtual int play() = 0;
	virtual ~Human() {
		std::cout << "Human 析构函数" << std::endl;
	}

protected:
	Human() {
		std::cout << "Human 构造函数" << std::endl;
	}
};


class Student : public Human {

	public:
		static Student * create();
		
		~Student() {
			std::cout << "Student 析构函数" << std::endl;
		}
		int play() {
			std::cout << "student play" << std::endl;
		}

	private:
		static Student * m_st;
		
		Student() {
			std::cout << "Student 构造函数" << std::endl;
		}
		
};


Student * Student::m_st;

Student * Student::create() {

	m_st = new Student();

	return m_st;
}

void test1() {

//	Human * h = Student::create();

	Human * h = Student::create();

	h->play();
	
}
int main() {
	
	test1();
//	test2();
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

