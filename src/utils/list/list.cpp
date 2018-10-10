#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include<sstream>
#include<list>

class Human {

public:
	Human() {}
	~Human() {}

	int m_id;
	std::string m_name;
};

class Teacher:public Human {

public:
	Teacher() {}
	~Teacher() {}
};

class Student:public Human {

public:
	Student() {}
	~Student() {}
};



std::list<std::string> test1_1() {

 	std::list<std::string> list1;

	std::string s = "zhaokun";
	
	std::cout << &s << std::endl;

    list1.push_back(s);
    list1.push_back("ycl");

	std::cout << "list1 = " << &list1 << std::endl;

	std::cout << &list1.front() << std::endl;
	return list1;
}

void test1() {

	std::list<std::string> list = test1_1();
	
//	list = test1_1();

	std::list<std::string>::iterator index = list.begin();

    for(index=list.begin();index!=list.end();index++) {
        std::cout<<"数据="<<*index<<std::endl;
    }

	std::cout << "list = " << &list << std::endl;

	std::cout << &list.front() << std::endl;
}

void test2() {
#if 0
	std::list<Human> list1;

	Human *h = new Human();

	h->m_id = 100;

	std::cout << "h = " << &h << std::endl;
    list1.push_back(h);

	std::cout << &list1 << std::endl;
	std::cout << &list1.front() << std::endl;
#endif
}

void test3() {

    std::list<int> c{0, 1, 2, 3, 0, 5, 0, 7, 8, 9};
    for (auto &i : c) {
        std::cout << i << " ";
    }
    std::cout << '\n';

	auto it = c.begin();

	for(;it!=c.end();it++) {
        if(*it == 0) {
			it = c.erase(it);
		}
    }

    for (auto &i : c) {
        std::cout << i << " ";
    }
    std::cout << '\n';

}

void test4() {

	Human *h1 = new Teacher();
	Human *h2 = new Student();

//	Teacher *h1 = new Teacher();
//	Student *h2 = new Student();

	std::cout << typeid(h1).name() << std::endl;
	std::cout << typeid(h2).name() << std::endl;

	char temp1[64] = {0};
	char temp2[64] = {0};

	snprintf(temp1,64,"%p",h1);
	snprintf(temp2,64,"%p",h2);

	std::cout << h1 << std::endl;
	std::cout << h2 << std::endl;

	printf("temp1 = %s\n",temp1);
	printf("temp2 = %s\n",temp2);

}

int main() {

//	test1();
//	test2();
//	test3();
	test4();
//	test5();
//	test6();
//	test7();
//	test8();
//	test9();
//	test10();

	while(1) {
		sleep(1);
	}
	return 0;
}

