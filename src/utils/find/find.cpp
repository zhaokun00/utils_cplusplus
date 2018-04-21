#include <stdio.h>
#include <iostream>
#include <thread>
#include <algorithm>

/*
	测试:std:find()函数

	template< class InputIterator, class T >
	InputIterator find( InputIterator first, InputIterator last, const T& value );
*/

//测试1:在普通数组中查找元素
void test1() {

	int nums[6] = {0,1,2,3,4,5};

	int start = 0;
	int end = 6;

	int findValue = 5;
	
	int *p = std::find(nums + start,nums + end,findValue);

	if(p == nums + end) {
		std::cout << "没有找到该元素" << std::endl;
	}
	else {
		std::cout << "value = " << *p << std::endl;
	}
} 

//测试在容器内查找元素
void test2() {

	std::vector<int> v;

	for(int i = 0;i < 100;i++) {
		v.push_back(i);
	}

	std::vector<int>::iterator it = std::find(v.begin(),v.end(),199);

	if(it != v.end()) {
		std::cout << "value = " << *it << std::endl;
	}
	else {
		std::cout << "没有找到该元素" << std::endl;
	}
	
}

//测试比较对象
class Human {

public:
	int id;

	Human() {}
	~Human() {}

	/*
		这个const指明了这个函数不会修改该类中的任何成员数据的值,称为常量成员函数,
		对于const函数的外部定义,也不能忘记写const限定符,例如:
		bool Human::operator==(const Human &rhs) const {

		}

		如果中间发现了修改成员变量,则编译时会报错
	*/
	bool operator==(const Human &rhs) const { //在测试比较对象时,要重写该方法
		return (this->id == rhs.id);
	}
};

void test3() {

	std::vector<Human> v;

	for(int i = 0;i < 100;i++) {
		Human h;
		h.id = i;
		v.push_back(h);
	}

	Human g;
	g.id = 89;
	
	std::vector<Human>::iterator it = std::find(v.begin(),v.end(),g);

	if(it != v.end()) {
		std::cout << "value = " << it->id << std::endl;
	}
	else {
		std::cout << "没有找到该元素" << std::endl;
	}
	
}

//测试比较对象的指针
typedef struct  finder_t {

	finder_t(int n) : id(n) {}

	bool operator()(Human *p) {
		return (id == p->id);
	}

	int id;
} finder_t;

void test4() {

	std::vector<Human *> v;

	for(int i = 0;i < 100;i++) {
		Human *h = new Human();
		h->id = i;
		v.push_back(h);
	}
	
	Human *g = new Human();

	g->id = 10;

	std::vector<Human *>::iterator it = std::find_if(v.begin(),v.end(),finder_t(g->id));

	if(it != v.end()) {
		std::cout << "value = " << (*it)->id << std::endl;
	}
	else {
		std::cout << "没有找到该元素" << std::endl;
	}
}

int main() {

//	test1();
//	test2();
//	test3();
	test4();

	return 0;
}

