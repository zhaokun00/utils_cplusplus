#include <stdio.h>
#include <memory>
#include   <iostream>

using namespace std;

/*
	unique_ptr"唯一"拥有其所指对象,同一时刻只能有一个unique_ptr指向给定对象(通过禁止拷贝语义、只有移动语义来实现)
	unique_ptr指针本身的生命周期:从unique_ptr指针创建时开始,直到离开作用域。离开作用域时,若其指向对象,则将其所指向对象销毁(默认使用delete操作符,用户可指定其操作)
	unique_ptr指针与其所指向对象的关系:在智能指针生命周期内,可以改变智能指针所指向对象,如创建智能指针时通过构造函数指定、通过reset方法重新指定、通过release方法释放
	所有权,通过移动语义转移所有权
*/
class Human {

	public:

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

//测试unique_ptr的初始化
void test1() {

	unique_ptr<Human> p1(new Human());

//	unique_ptr智能指针中没有该函数
//	cout << "use count " << p1.use_count()<<endl;

}

//测试unique_ptr的移动
void test2() {

	unique_ptr<Human> p1(new Human());

	p1->id = 10;
	//1.这种复制不可以
//	unique_ptr<Human> p2(p1); //在shared_ptr中可以

	//2.这种赋值不可以
//	unique_ptr<Human> p2; //在shared_ptr中可以
//	p2 = p1;

	//3.正确的赋值方式

	unique_ptr<Human> p2 = std::move(p1);

	cout << "id = " << p2->id << endl;
}

//测试release函数
void test3() {

	unique_ptr<Human> p1(new Human());

	p1->id = 10;

	//成员函数release,这个函数只是把智能指针赋值为空,但是它原来的内存并没有释放,相当于它只是释放了对资源的所有权
	p1.release();

	if(p1.get() == NULL) { //由于前面已经调用了release函数,因此会把指针指针赋值为空
		printf("pointer is null\n");
	}
	else {
		printf("pointer is not null\n");
	}

	//最终并不会调用Human类的析构 函数
	
}

//测试reset函数
void test4() {

	unique_ptr<Human> p1(new Human());

	p1->id = 10;

//	p1.reset(); //reset()函数会重新绑定指向的对象,而原来的对象会被释放,如果不传入参数则会指向NULL
	p1.reset(new Human()); //也可以重新绑定新的对象
	if(p1.get() == NULL) {
		printf("pointer is null\n");
	}
	else {
		printf("pointer is not null\n");
	}
}

//测试调用std::move函数时会不会释放掉原来指向的空间.答案:会释放掉原来的空间
void test5() {

	unique_ptr<Human> p1(new Human());
	unique_ptr<Human> p2(new Human());

	p2 = std::move(p1); //p2会释放掉原来的指向的空间

	printf("进行了转移操作\n");
}

void test6() {

	Human h1(); //这种方式不会调用构造函数,感觉是一个函数的调用
	Human h2; //这种方式会调用构造 函数

}


//测试:make_unique函是C++14的标准,在C++11中不能使用,如果要实现可以自己实现以下
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

void test7() {

	unique_ptr<Human> p1;
	
	p1 = make_unique<Human>();
	
}

//测试:形参类型为unique_ptr<Human>类型,当形参类型为unique_ptr<Human>类型时,传递参数也必须用std:move函数进行传递,否则会报错
void test8_1(unique_ptr<Human> p) {

}

void test8() {

	unique_ptr<Human> p1(new Human());

//	test8_1(p1); //直接调用该函数时会报错

	test8_1(std::move(p1)); //正确的调用方式
}

//测试:形参类型为unique_ptr<Human> &类型
void test9_1(unique_ptr<Human> &p) {

}

void test9() {

	unique_ptr<Human> p1(new Human());

	test9_1(p1); //当传递的参数为unique_ptr<Human> &类型时,可以直接这样进行传递
}

//测试返回的数据类型:unique_ptr<Human>
unique_ptr<Human> test10_1() {

	unique_ptr<Human> p(new Human());

	return p;
}

void test10() {

	unique_ptr<Human> p(new Human());

	p = test10_1();

	printf("test\n");
	
}

/*
	返回非引用类型:
		1.函数的返回值用于初始化在调用函数时创建的临时对象,如果返回类型不是引用,在调用函数的地方会将函数返回值复制给临时对象
		2.在求解表达式的时候,如果需要地方存储其运算结果,编译器会创建一个没有命名的对象,这就是临时对象
		3.用函数返回值初始化临时对象与用实参初始化形参的方法是一样的
		4.当函数返回非引用类型时,其返回值既可以是局部对象,也可以是求解表达式的结果
		
	返回引用类型:
		1.当函数返回引用类型时,没有复制返回值,相反返回的是对象本身
		2.千万不要返回局部对象的引用,千万不要返回指向局部对象的指针
			当函数执行完毕后,将释放分配给局部对象的存储空间,此时对局部对象的引用就会指向不确定的内存,返回指向局部对象的指针也是一样,当函数结束时,
		局部对象被释放,返回的指针就变成了不存在的对象的悬垂指针
		3.返回引用时,要求在函数的参数中,包含有以引用方式或指针方式存在的需要被返回的参数
*/
//测试:返回引用类型,正确的返回引用,参数中传入了需要返回的引用参数
Human & test11_1(Human & h) {

	h.id = 10;

	return h;
}

//错误的使用方法:返回引用类型
Human & test11_2() {

	Human h;

	h.id = 100;

	return h;
}

void test11() {

	Human h;

	h.id = 9;

//	h = test11_1(h); //正确的方式

	h = test11_2();

	printf("id = %d\n",h.id);
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
	test11();
	return 0;
}

