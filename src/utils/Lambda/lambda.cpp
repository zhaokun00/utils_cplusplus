#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include<sstream>
/*
Lambda作用:
	用于创建匿名函数
举例:
	标准C++库中哟iu一个常用算法的库,其中提供了很多算法函数,比如sort()和find(),这些函数通常需要提供一个“谓词函数”。所谓谓词函数,就是
进行一个操作作用的临时函数,比如find()需要一个谓词,用于查找元素满足的条件,能够满足谓词函数的元素才会被查找出来。这样的谓词函数,使用临时的匿名函数,
可以减少函数数量,又会让代码清晰易读

声明Lambda表达式:
[capture list] (params list) mutable exception-> type { function body }

1.capture list:捕获外部变量的列表
2.params list:形参列表
3.mutable指使符:用来说是否可以修改捕获的变量
4.exception:异常设定
5.type:返回类型
6.function body:函数体

此外,我们还可以省略其中的某些成分来声明不完整的Lambda表达式,常见的有以下几种

序号    		格式
1			[capture list] (params list) -> return type {function body}
2			[capture list] (params list) {function body}
3			[capture list] {function body}

其中:
格式1声明了const类型的表达式,这种类型的表达式不能修改捕获列表中的值
格式2省略了返回值类型,但编译器可以根据以下规则推断出Lambda表达式的返回类型
(1)如果function body存在return语句,该Lambda表达式的返回类型由return的返回类型确定
(2)如果function body中没有return语句,则返回值为void类型
格式3中省略了参数列表,类似普通函数中的无参函数
*/

//Lambda简单使用
void test0() {

//格式1:
//	auto func = []()-> int {return 100;}; //用于定于lambda表达式的定义

//	std::cout << "func = " << func() << std::endl; //调用lambda表达式

//格式2:
//	auto func = [](){return 99;};

//	std::cout << "func = " << func() << std::endl;

//格式3:
	auto func = [](){std::cout << "Lambda" << std::endl;};

	func();
}

/*
Lanbda表达式可以使用其可见范围内的变量,但必须明确声明(明确声明哪些外部变量可以被Lambda表达式使用),
那么,在哪里指定这些外部变量呢？Lambda表达式通过最前面的方括号[]来明确指明其内部可以访问的外部变量,
这一过程称为Lambda表达式捕获了外部外部变量
*/
//捕获外部变量,按值进行捕获
void test1() {

//测试auto和auto *类型,二者是一样的
#if 0
	int a = 10;

	auto p =  &a;

	std::cout << *p << std::endl;

	auto * p1 = &a;
	
	std::cout << *p1 << std::endl;
#endif

	int a = 123;

	//如果按值进行捕获则在lambda表达式函数体中不能修改外部变量的值,将数据进行复制
	auto func = [a](){
		
//		a = 100; //在这里修改时会报错
		std::cout << a << std::endl;

	};

	a = 100;
	func(); //输出为123

	std::cout << a << std::endl;
}

//引用捕获
void test2() {

	int a = 123;

	auto func = [&a](){
		
		std::cout << a << std::endl;
		a = 100; //传递引用时可以修改值
	};

#if 0
	a = 100;
	func(); //输出为100
#endif

	func();
	std::cout << a << std::endl;
}

//在函数体内定义变量
void test3() {

	int a = 123;

	auto func = [&a](){
		
		int a = 90; //会覆盖引入的变量a
		int b = 80; //函数体内定义变量
		std::cout << a << std::endl; //输出90
		std::cout << b << std::endl;
	};

	a = 100;
	func(); //输出为100

	std::cout << a << std::endl;
}

/*
上面的值捕获和引用捕获都需要在捕获列表中显示出Lambda不表达式中使用的外部变量,除此之外，我们还可以让编译器根据函数体中的代码来
推断需要捕获哪些变量,这种称之为隐式捕获。隐式捕获有两种方式,分别是[=]和[&],[=]表示以值捕获的方式捕获外部变量,[&]表示以引用捕获的方式
捕获外部变量
*/
//隐式捕获-值
void test4() {

	int a = 10;

	auto func = [=](){std::cout << a << std::endl;};

	func();
}


//隐式捕获-引用
void test5() {

	int a = 10;

	auto func = [&](){

		std::cout << a << std::endl;

	};

	a = 20;

	func();
}

//事例：
void test6() {

	int a = 10;
	int b = 20;
	int *c = &a;

	auto func = [a,b,c](int x,int y) {

		std::cout << a << std::endl;
		std::cout << b << std::endl;
		std::cout << *c << std::endl;

		*c = 90;
//不能修改
#if 0
		int d = 30;

		c = &d;
#endif
		return x + y;
	};

	auto sum = func(1,2);

	std::cout << a << std::endl; //输出90

	std::cout << sum << std::endl;
}

/*
捕获形式	说明
[]	不捕获任何外部变量
[变量名, …]	默认以值得形式捕获指定的多个外部变量（用逗号分隔），如果引用捕获，需要显示声明（使用&说明符）
[this]	以值的形式捕获this指针
[=]	以值的形式捕获所有外部变量
[&]	以引用形式捕获所有外部变量
[=, &x]	变量x以引用形式捕获，其余变量以传值形式捕获
[&, x]	变量x以值的形式捕获，其余变量以引用形式捕获
*/

/*
在Lambda表达式中,如果以传值方式捕获外部变量,则函数体中不能修改该外部变量,否则会引发编译错误,如果要修改捕获的外部变量,就需要使用mutable关键字,
该关键字用于说明表达式体内的代码可以修改值捕获的变量。要想修改值可以通过1.传递引用 2.传递指针
*/
//修改捕获的变量
void test7() {

	int a = 123;

	auto func = [a]() mutable { //mutable并不能改变外部变量的值
		a = 100;

		std::cout << a << std::endl; //输出100
	};

	func(); //输出123

	std::cout << a << std::endl;
}

//在线程中使用lambda表达式
void test8() {

	std::thread t([](){
		
		std::cout << "thread" << std::endl;
	});

	std::cout << "begin" << std::endl;
	t.detach();
	std::cout << "end" << std::endl;
}

/*
Lambda表达式中的参数:
Lambda表达式的参数和普通函数类似,但Lambda表达式中传递参数还有一些限制,主要有以下几点:
1.参数列表中不能有默认参数
2.不支持可变参数
3.所有参数必须有参数名
*/
void test9() {

	std::thread t([](int x,int y){
		
		std::cout << "thread" << std::endl;

		std::cout << x + y << std::endl;

	},2,3);

	std::cout << "begin" << std::endl;
	t.detach();
	std::cout << "end" << std::endl;
}

int main() {

//	test0();
//	test1();
//	test2();
//	test3();
//	test4();
//	test5();
//	test6();
//	test7();
//	test8();
	test9();
//	test10();

	while(1) {
		sleep(1);
	}
	return 0;
}

