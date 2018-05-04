#include <stdio.h>
#include <memory>
#include   <iostream>
#include <map>
using namespace std;

/*
	shared_ptr的使用:
	shared_ptr多个指针指向相同的对象,shared_ptr使用引用计数,每一个shared_ptr的拷贝都指向相同的内存。每次使用它一次,内部的引用的计数加1,
	每析构一次,内部引用计数减1,减为0时,自动删除所指向的堆内存。shared_ptr内存的引用计数是线程安全的,但是对象的读取需要加锁

	1.初始化:智能指针时一个模板类,可以指定类型,传入指针通过构造函数初始化,也可以使用make_shared函数初始化,不能讲指针指针直接赋值给一个智能指针,因为
	一个是类,一个是指针。例如:shared_ptr<int> p = new int(1),这种写法是错误的

	2.拷贝和赋值,拷贝使得对象的引用计数加1,赋值使得原有对象引用计数减1,当计数为0时,自动释放内存

	3.get函数获取原始指针

	4.注意不要一个原始指针初始化多个shared_ptr,否则会造成二次释放同一内存

	5.注意避免循环引用,shared_ptr的一个最大陷阱是循环引用,循环引用会导致堆内存无法正确释放,导致内存泄漏
*/

class Human {

	public:

		/*
			注意:构造函数要声明为public类型

			所遇到的问题:
				1.构造函数没有声明为public类型,在调用make_shared函数时报错
		*/
		Human() {
			cout<<"调用了无参构造函数"<<endl;
		}

		Human(int id) {
			this->id = id;
			cout<<"调用了有参构造函数"<<endl;
		}
		//重写了复制构造函数
/*
		Human(Human &h) {
			this->id = h.id
		}
*/
		~Human() {
			cout<<"调用了析构函数"<<endl;
		}
		int id;
};

//shared_ptr智能指针的初始化
void test1() {

/*
	make_shared函数只要功能是在动态内存分配中分配一个对象并初始化它,返回指向此对象的shared_ptr,由于
	是通过shared_ptr管理内存,因此是一种安全分配和使用的动态内存的方法

	(1)make_shared是一个模板函数
	(2)make_shared模板的使用需要以"显示模板实参的方式使用",例如使用带参的构造函数
	(3)make_shared在传递参数格式是可变的,参数传递为生成类型的构造函数参数,因此在创建shared_ptr<T>对象的过程中调用了类型T的某个构造函数
*/
//第一种方式
#if 0
	int a = 10;
	//使用make_shared函数来初始化一个智能指针
    shared_ptr<int> ptra = make_shared<int>(a);
	printf("a = %d\n",*ptra);

//第一种方式

	//会调用无参构造函数
	shared_ptr<Human> p1 = make_shared<Human>();
	p1->id = 10;

	cout << "id = "<<p1->id <<endl;
	
	//会调用有参构造函数
	shared_ptr<Human> p2 = make_shared<Human>(5);
	cout << "id = "<<p2->id <<endl;
#endif

//第二种方式
#if 0
	Human * h = new Human(); //直接创建了Human类型的指针

	shared_ptr<Human> p1(h); //将创建的Human类型指针赋值给指针指针

	cout << "use count " << p1.use_count()<<endl; //打印当前引用该对象的个数
#endif

//第三种方式
	shared_ptr<Human> p1(new Human()); //直接在定义智能指针时初始化对象

	cout << "use count " << p1.use_count()<<endl; //打印当前引用该对象的个数

	p1->id = 100;

	cout << "id = "<<p1->id <<endl;
	
}

//注意不要一个原始指针初始化多个shared_ptr,否则会造成二次释放同一内存
void test2() {

	Human * h = new Human(); //会调用一次构造函数

	//用同一个指针初始化两个智能指针,这里会调用2次Human类的析构函数,会造成二次释放(备注:虽然造成了二次释放,但程序并未崩溃)
	shared_ptr<Human> p1(h); //当test2()函数结束时会调用一次Human类的析构函数
	shared_ptr<Human> p2(h); //当test2()函数结束时会调用一次Human类的析构函数
}

//获取原始指针
void test3() {

	Human * h1 = new Human();

//	Human * h2 = h1; //第一种方式赋值,这种方式会调用Human类的复制构造函数

	Human * h2 = NULL; 
	h2 = h1; //第二种方式,这种方式会调用Human类的赋值函数

	//C语言的打印方式
//	printf("h1 = %p,h2 = %p\n",h1,h2); //打印的是h1、h2中存储的地址,二者是相同的

	//C++语言的打印方式
	cout << "h1 = " << h1 << "h2 = " << h2 <<endl;

//	printf("&h1 = %p,&h2 = %p\n",&h1,&h2); //打印h1所在的内存地址
//	cout << "h1 = " << &h1 << "h2 = " << &h2 <<endl; //打印h2所在的内存地址
	
	shared_ptr<Human> p1(h1);

	Human * h3 = p1.get(); //获取原始指针

	cout << "h3 = " << h3 <<endl;
	
}

//智能智能的拷贝和赋值
void test4() {

	Human * h1 = new Human();

	shared_ptr<Human> p1(h1);

	cout << "use_count:" << p1.use_count() << endl; //count = 1

	shared_ptr<Human> p2(p1); //会调用智能指针的复制构造函数

	cout << "use_count:" << p1.use_count() << endl; //count = 2

	shared_ptr<Human> p3;

	p3 = p2; //会调用智能指针的赋值函数(operate =),count = 3

	cout << "use_count:" << p1.use_count() << endl;

	shared_ptr<Human> p4(new Human());

	p4 = p3; //赋值函数,会导致释放掉原有的引用,使原有的引用的count值减1,使现有的count值加1

	cout << "use_count:" << p1.use_count() << endl; //count = 4
	
}

//智能指针的reset函数
void test5() {

	Human * h1 = new Human();

	shared_ptr<Human> p1(h1);

	shared_ptr<Human> p2(p1);

	cout << "use_count:" << p1.use_count() << endl;

	p1.reset(); //reset函数重新绑定指向对象,会释放掉原来引用,count值减1

//	p1.reset(new Human()); //可以绑定到其他的引用
	
	cout << "use_count:" << p2.use_count() << endl; //count = 1
}

//智能指针的release函数
void test6() {

	Human * h1 = new Human();

	shared_ptr<Human> p1(h1);

	cout << "use_count:" << p1.use_count() << endl;

//	p1.release(); //对于shared_ptr并没有release函数

	cout << "use_count:" << p1.use_count() << endl;

}

//测试形参为智能指针对象
void test7_1(shared_ptr<Human> p) { //会调用智能指针的复制构造函数,引用计数加1

	cout << "use_count2:" << p.use_count() << endl; //打印count = 2,当函数结束后又会释放引用,count值减1
}

void test7() {

	Human * h1 = new Human();

	shared_ptr<Human> p1(h1);

	cout << "use_count1:" << p1.use_count() << endl; //打印的count = 1

	test7_1(p1);

	cout << "use_count1:" << p1.use_count() << endl; //打印count = 1
}

//测试形参为指针指针的引用
void test8_1(shared_ptr<Human> &p) { //传递的为引用类型,不会调用复制构造函数,也不会调用构造函数,因此不会调用析构函数,因此count值不会发生变化

	cout << "use_count2:" << p.use_count() << endl; //count = 1,函数结束后也不会调用析构函数,因为根本就没有发生构造
}

void test8() {

	Human * h1 = new Human();

	shared_ptr<Human> p1(h1);

	cout << "use_count1:" << p1.use_count() << endl; //打印count = 1

	test8_1(p1);

	cout << "use_count1:" << p1.use_count() << endl; //打印count = 1
}

//测试传递引用类型时是否会调用构造函数或复制构造函数
void test9_1(Human &h) { //不会

}

void test9() {
	Human h; //会调用构造函数
	test9_1(h);
		
}

//测试类中使用智能指针
class TestHuman {

	public:
		shared_ptr<Human> m_human;
};

void test10_1(TestHuman &h) {

	Human * h1 = new Human();

	shared_ptr<Human> p1(h1);

	cout << "use_count1:" << p1.use_count() << endl; //打印count = 1
	h.m_human = p1;
	cout << "use_count2:" << p1.use_count() << endl; //打印count = 2
}

void test10() {

	TestHuman th;

	test10_1(th);

	cout << "use_count3:" << th.m_human.use_count() << endl; //打印count = 1,在函数结束时会释放掉在test10_1中创建的Human *指针
}

//测试智能指针的多态问题
class Animal {

	public:
		Animal() {
			std::cout << "Animal 构造函数" << std::endl;
		}
		virtual ~Animal() {
			std::cout << "Animal 析构函数" << std::endl;
		}
		virtual void play() {
			std::cout << "Animal play" << std::endl;
		}
};

class Dog:public Animal {

	public:
		Dog() {}
		Dog(int id):m_id(id){}
		virtual ~Dog() {}
		virtual void play() {
			std::cout << "Dog play" << std::endl;
		}
		int m_id;
};

void test11() {

#if 0
	Animal * al = new Animal();

	Dog * dog = new Dog();

	al->play();
	dog->play();
#endif

	shared_ptr<Animal> al(new Dog(100));

//	shared_ptr<Dog> dog = (shared_ptr<Dog>)al; //不能这样转换
	
	al->play();

	std::cout << ((Dog *)al.get())->m_id << std::endl; //可以这样获取到原始指针,然后获取参数
//	std::cout << al->m_id << std::endl;

//	dog->play();
	
}

//测试智能指针不赋值时是否为空
void test12() {

	shared_ptr<Animal> al; //没有初始化时,智能指针为空

	if(NULL == al) {
		std::cout << "智能指针为空" << std::endl;
	}
	else {
		std::cout << "智能指针不为空" << std::endl;
	}

	shared_ptr<Animal> a(new Animal()); //初始化时,智能指针不为空
	if(NULL == a) {
		std::cout << "智能指针为空" << std::endl;
	}
	else {
		std::cout << "智能指针不为空" << std::endl;
	}
}

void test13() {

	map<int,shared_ptr<Animal>> m1;

	shared_ptr<Animal> p1(new Animal());

	std::cout << "第一次cout = " << p1.use_count() << std::endl;
	
	m1.insert(make_pair(1,p1));

	std::cout << "第二次cout = " << p1.use_count() << std::endl;
}
int main() {

//	test1();
//	test2();
//	test3();
//	test4();
//	test5();
//	test6();
//	test7();
//	test8();
//	test9();
//	test10();
//	test11();
//	test12();
	test13();

	return 0;
}

