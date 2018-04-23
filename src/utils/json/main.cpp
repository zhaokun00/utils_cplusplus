#include <stdio.h>
#include <iostream>
#include <chrono>
#include"json.hpp"
//using namespace std;
//using json=nlohmann::json;

//https://blog.csdn.net/forrid/article/details/78867679
//https://blog.csdn.net/fengxinlinux/article/details/71037244

//测试:std::cin函数,读取终端输入的字符串
void test1() {

	std::string str;
#if 1
	std::cout << "输入字符串:";

	//将输入的字符串赋值给str变量
	std::cin >> str;

	std::cout << str << std::endl;
#endif

	std::string s = "q";

	if(s == str) {
		std::cout << "两个字符串相等" << std::endl;
	}
	else {
		std::cout << "两个字符串不相等" << std::endl;
	}
}

void test2() {

	nlohmann::json j;

	j["pi"] = 3.14;
	j["happy"] = true;
	j["answer"]["everything"] = 42;

	std::cout << j << std::endl;

//不能这样直接进行赋值
#if 0
	std::string str = j;

	std::cout << str << std::endl;
#endif

#if 0
	std::string str = j.dump();
	std::cout << str << std::endl;
#endif

#if 0
 	std::string str = j["pi"].get<std::string>();

	//错误的形式
//	std::string str = j["pi"].get();

	std::cout << str << std::endl;
#endif

#if 0
	std::string str = j.dump();
	nlohmann::json jj = nlohmann::json::parse(str);

	std::cout << jj << std::endl;
#endif

	std::cout << j["happy"].get<bool>() << std::endl;
	std::cout << j["answer"]["everything"].get<int>() << std::endl;
}

int main() {

//	test1();
	test2();
//	test3();
//	test4();
//	test5();
//	test6();

	return 0;
}

