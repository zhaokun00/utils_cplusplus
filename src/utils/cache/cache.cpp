#include <stdio.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <map>
#include <list>
#include <algorithm>    // transform

//https://blog.csdn.net/coder_yi_liu/article/details/49453827
class Human {

	public:
		Human() {
			
		}
		~Human() {
			
		}

		void print() {
			printf("human\n");
		}
};

class Person {

	public:
		Person() {
			
		}
		~Person() {
			
		}

		void prin() {
			printf("Person\n");
		}
};

class student : public Human,public Person {

	public:
		student() {
			
		}
		~student() {
			
		}
	
};

void test1() {

	Human * h = new student();

// 直接进行转换
	((Person *)h)->prin();
}

int main() {

	test1();

	return 0;
}

