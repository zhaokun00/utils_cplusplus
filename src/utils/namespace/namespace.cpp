#include <stdio.h>
#include <iostream>
#include <chrono>

#include "Hello.h"

/*
	尽量不要使用using namesoace std
	C++标准程序库中的所有标识符都被定义于一个名为std的namespace中,namespace是指标识符的各种可见范围,命名空间用关键字namespace来定义,
	命名空间是C++中的一种机制,用来把单个标识符下的大量有逻辑联系的程序组合在一起	
*/
//using namespace std;

//C++中引入命名空间的作用是为了避免命名冲突问题

//测试具有单个命名空间的情况
void test1() {

	HelloService::Hello h;

	h.hello();
}

//测试具有多个命名空间的情况
void test2() {

	HelloService::GoodByeService::GoodBye g;

	g.goodBye();
}

/*
  1.如果使用using namespace std;
  	在定义cout(),会与std命名空间中的cout函数发生冲突,因为该函数将来也属于命名空间std中

  2.如果没有使用using namespace std;
    则可以定义cout函数
*/

void cout() {

}

void test3() {

	cout();
}

int main() {

//	test1();
//	test2();

	test3();
	return 0;
}

