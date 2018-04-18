#include <stdio.h>
#include <iostream>
#include <chrono>
#include <stack>

/*
	C++中的stack(栈)是一个容器的改编,它实现了一个先进后出的数据结构(FILO),使用该容器时需要包含头文件
	#include <stack>头文件

	stack的基本操作有:
	1.入栈
		s.push(x)
	2.出栈
		s.pop()
	3.访问栈顶
		s.top()
	4.判断栈是否为空,当栈为空时返回true
		s.empty()
	5.访问栈中的元素个数
		s.size()
*/

void test1() {

	std::stack<std::string> s;

	s.push("zhao"); //入栈
	s.push("qian");
	s.push("sun");

	std::cout << "size = " << s.size() << std::endl;;

	std::cout << "top = " << s.top() << std::endl;

	s.pop(); //出栈

	std::cout << "top = " << s.top() << std::endl; //获取栈顶栈顶元素

	if(s.empty()) {	
		std::cout << "栈为空" << std::endl;
	}
	else {
		std::cout << "栈不为空" << std::endl;
	}
}

class Human {

public:
	Human():id(10){}
//	Human(const Human & h) { std::cout << "复制构造函数" << std::endl;}
	int id;
};

void test2() {

	std::stack<Human> s;

	Human h;

	h.id = 100;
	
	s.emplace(h);

	std::cout << "top = " << s.top().id << std::endl; //获取栈顶栈顶元素

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

