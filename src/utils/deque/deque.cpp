#include <stdio.h>
#include <iostream>
#include <deque>
#include <chrono>
/*
	deque容器为一个给定类型的元素进行线性处理,向向量一样,它能够快速地随机访问任何一个元素,并且能够高效
	地插入和删除容器的尾部元素,但是它与vector不同,deque支持高效插入和删除容器的头尾部元素,因此也叫双端队列
*/

//测试:向双端队列中添加元素
void test1() {

	std::deque<int> d;

	d.push_back(10); //向双端队列尾部添加元素
	d.push_back(20);
	d.push_back(30);

//遍历的第一种方式
	for(int i = 0; i < d.size();i++) {
		/*
			原型:
			reference	at( size_type pos );

				pos	-	position of the element to return
			
				Return value:
				reference to the requested element
		*/
		std::cout << d.at(i) << std::endl;
	}

//遍历的第二种方式
	std::deque<int>::iterator index;
	
	for(index = d.begin();index < d.end();index++) {
		std::cout << "index=" << *index << std::endl;
	}
	
}

//测试:向双端队列中添加对象
class Human {

public:
	Human():id(10){}
	Human(const Human & h) { std::cout << "复制构造函数" << std::endl;}
	int id;
};

std::deque<Human> g_test2_d;
void test2_1() {

	Human h1;
	Human h2;

	h1.id = 1000;
	h2.id = 2000;

	std::cout << "add1 = " << &h1 << std::endl;

	g_test2_d.push_back(h1); //会调用复制构造函数,会复制一份数据存在容器中
	g_test2_d.push_back(h2);

	h1.id = 300;

	std::cout << "add2 = " << &g_test2_d.at(0) << std::endl;
	std::cout << "data = " << g_test2_d.at(0).id << std::endl;
	
}

void test2() {

	test2_1();

	std::cout << "size = " << g_test2_d.size() << std::endl;
	
}

//测试:向双端队列中插入元素
/*
	void push_front(const T& x):双端队列头部增加一个元素X

	void push_back(const T& x):双端队列尾部增加一个元素x

	iterator insert(iterator it,const T& x):双端队列中某一元素前增加一个元素x

	void insert(iterator it,int n,const T& x):双端队列中某一元素前增加n个相同的元素x

	void insert(iterator it,const_iterator first,const_iteratorlast):双端队列中某一元素前插入另一个相同类型向量的[forst,last)间的数据

*/
void test3() {

	std::deque<std::string> d;

	d.push_back("end"); //向尾端添加元素
	d.push_front("start"); //向首端添加元素

	std::deque<std::string>::iterator it = d.begin();
	d.insert(it,"middle");
	
	for(int i = 0;i < d.size();i++) {

		std::cout << d.at(i) << std::endl;
	}
}

//测试:双端队列的删除函数
/*
	iterator erase(iterator it):删除双端队列中的某一个元素

	iterator erase(iterator first,iterator last):删除双端队列中[first,last）中的元素

	void pop_front():删除双端队列中最前一个元素

	void pop_back():删除双端队列中最后一个元素

	void clear():清空双端队列中最后一个元素
*/
void test4() {

	std::deque<std::string> d;

	d.push_back("1"); //向尾端添加元素
	d.push_back("2"); //向尾端添加元素
	d.push_back("3"); //向尾端添加元素
	d.push_back("4"); //向尾端添加元素
	d.push_back("5"); //向尾端添加元素

	std::cout << "添加开始" << std::endl;
	for(int i = 0;i < d.size();i++) {

		std::cout << d.at(i) << std::endl;
	}

	std::cout << "删除开始元素" << std::endl;

	d.pop_front(); //删除头元素
	
	for(int i = 0;i < d.size();i++) {

		std::cout << d.at(i) << std::endl;
	}

	std::cout << "删除尾元素" << std::endl;
	d.pop_back(); //删除尾元素
	for(int i = 0;i < d.size();i++) {

		std::cout << d.at(i) << std::endl;
	}

	std::cout << "删除特定元素" << std::endl;
	//清除特定的元素
	d.erase(d.begin() + 1);
	for(int i = 0;i < d.size();i++) {

		std::cout << d.at(i) << std::endl;
	}
		
	std::cout << "清空容器" << std::endl;
	d.clear();
	for(int i = 0;i < d.size();i++) {

		std::cout << d.at(i) << std::endl;
	}

	std::cout << "删除结束" <<std::endl;
}

//测试:双端队列的遍历函数
/*
reference at(int pos):返回pos位置元素的引用

reference front():返回首元素的引用

reference back():返回尾元素的引用

iterator begin():返回向量头指针，指向第一个元素

iterator end():返回指向向量中最后一个元素下一个元素的指针（不包含在向量中）

reverse_iterator rbegin():反向迭代器，指向最后一个元素

reverse_iterator rend():反向迭代器，指向第一个元素的前一个元素

*/
void test5() {

	std::deque<std::string> * d = new std::deque<std::string>();

	d->push_back("1");
	d->push_back("2");
	d->push_back("3");
	d->push_back("4");
	d->push_back("5");
	

	std::deque<std::string>::iterator it = d->end() - 1;

	std::cout << *it << std::endl;
	
	delete d;
	d = NULL;
}

/*
(5)    判断函数

bool empty() const:向量是否为空，若true,则向量中无元素

(6)    大小函数

Int size() const:返回向量中元素的个数

int max_size() const:返回最大可允许的双端对了元素数量值

(7)    其他函数

void swap(deque&):交换两个同类型向量的数据

void assign(int n,const T& x):向量中第n个元素的值设置为x

*/
int main() {

//	test1();
//	test2();
//	test3();
//	test4();
	test5();
//	test6();
//	test7();
//	test8();
//	test9();
//	test10();
	return 0;
}

