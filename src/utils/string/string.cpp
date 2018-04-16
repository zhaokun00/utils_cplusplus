#include <stdio.h>
#include <iostream>
#include <thread>
#include <string>
#include <chrono>

//https://blog.csdn.net/tengfei461807914/article/details/52203202
void test1_1(std::string s) {

	s = "yangcuiling";

	std::cout << s << std::endl;

}

void test1_2(std::string & s) {

	s = "yangcuiling";

	std::cout << s << std::endl;

}

std::string test1_3() {

	std::string s = "yangcuiling";

	return s;
}
void test1() {

	std::string s = "zhaokun";

//	test1_1(s);

//	test1_2(s);

	s = test1_3();

	std::cout << s << std::endl;
}

void test2() {

	std::string s = new std::string();

	
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

