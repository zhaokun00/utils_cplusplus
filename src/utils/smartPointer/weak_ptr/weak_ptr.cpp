#include <stdio.h>
#include <memory>
#include   <iostream>

using namespace std;

/*
	weak_ptr是为了配合shared_ptr而引入的一种智能指针,因为它不具备普通指针的行为,没有重载operator*和->,它的最大作用是协助shared_ptr工作,像旁观者
	那样观测资源的使用情况.weak_ptr可以从一个shared_ptr或者另一个weak_ptr对象构造,获取资源的观测全。但weak_ptr没有共享资源,它的构造不会引起指针引用计数的
	增加,使用weak_prt的成员函数use_count()可以观测资源的引用计数,另一个成员函数expired的功能等价于use_count() == 0,但更快,表示被观测的资源(也就是shared_ptr的
	管理资源)不复存在,weak_ptr可以使用一个非常重要的成员函数lock()从被观测的shared_ptr获得一个可用的shared_ptr对象,从而操作资源,但当expired()==true时,lock()函数
	将返回一个存储空指针的shared_ptr
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

//weak_ptr的初始化
void test1() {

	shared_ptr<Human> sp(new Human());

	cout << "sp count:" << sp.use_count() << endl; //count = 1

	weak_ptr<Human> wp(sp); //weak_ptr只是获得了观测权,但没有共享资源

	cout << "sp count:" << sp.use_count() << endl; //count = 1
	cout << "wp count:" << wp.use_count() << endl; //count = 1
}

//expired()函数的使用,检查当前对象的计数是否为0
void test2() {

	shared_ptr<Human> sp(new Human());
	weak_ptr<Human> wp(sp);

	cout << "sp count:" << sp.use_count() << endl;

	if(wp.expired()) { //判断use_count == 0,如果为0则返回true,不为0返回false,不会进入该if语句

		cout << "no use" <<endl;
	}

	cout << "===分割线===" << endl;
	sp.reset();

	if(wp.expired()) { //由于前面已经reset了,因此会进入该if语句

		cout << "no use" <<endl;
	}
}

//lock()函数的作用:从被观测的shared_ptr获得一个可用的shared_ptr对象
void test3() {

	shared_ptr<Human> sp(new Human());
	weak_ptr<Human> wp(sp);

	cout << "sp count:" << sp.use_count() << endl; //count = 1

	shared_ptr<Human> sp1 = wp.lock(); //获取一个shared_ptr对象

	cout << "sp count:" << sp.use_count() << endl; //count = 2
}

//解决循环引用的问题
#if 0
class Parent;
class Child;

class Parent {

private:
	Child * ch;

public:
	Parent() {
		cout << "Parent构造函数" <<endl;
	}
	~Parent() {
		cout << "Parent析构函数" <<endl;
		if(ch) {
			delete ch;
			ch = NULL;
		}
		else {
			cout << "ch is null" << endl;
		}
	}

	void setChild(Child * c) {
		ch = c;
	}
};

class Child {

private:
	Parent * pa;
public:
	Child() {
		cout << "Child构造函数" <<endl;
	}
	~Child() {
		cout << "Child析构函数" <<endl;
		if(pa) {
			delete pa;
			pa = NULL;
		}
	}
	void setParent(Parent * p) {
		pa = p;
	}
};

void test4() {

	Parent * p = new Parent();
	Child *  c = new Child();

	p->setChild(c);
	c->setParent(p);

	delete p;
	p = NULL;

//	delete c;
//	c = NULL;
	
}
#else 
#if 0
class Parent;
class Child;

class Parent {

private:
//	shared_ptr<Child> child;
	weak_ptr<Child> child;
public:
	Parent() {
		cout << "Parent构造函数" <<endl;
	}
	~Parent() {
		cout << "Parent析构函数" <<endl;
	}

	void setChild(shared_ptr<Child> s) {
		child = s;
	}
};

class Child {

private:
	shared_ptr<Parent> parent;
	
public:
	Child() {
		cout << "Child构造函数" <<endl;
	}
	~Child() {
		cout << "Child析构函数" <<endl;
	}
	void setParent(shared_ptr<Parent> s) {
		parent = s;
	}
};

void test4() {

	weak_ptr<Parent> wp;
	weak_ptr<Child> wc;
{
	shared_ptr<Parent> parent(new Parent());
	shared_ptr<Child> child(new Child());

	wp = parent;
	wc = child;
	
	parent->setChild(child);
	child->setParent(parent);

//两个都声明为shared_ptr类型
	cout << "parent' s count" << parent.use_count() << endl; //count = 2,count = 2
	cout << "child' s count" << child.use_count() << endl; //count = 2,count = 1
}

	cout << "parent' s count" << wp.use_count() << endl; //count = 1,count = 1
	cout << "child' s count" << wc.use_count() << endl; //count = 1,count =1 

}
#endif
#endif

template<typename T>
class SmartPointer {
private:
     T* _ptr;
     size_t* _count;
public:
     SmartPointer(T* ptr = nullptr) :
             _ptr(ptr) {
         if (_ptr) {
             _count = new size_t(1);
         } else {
             _count = new size_t(0);
         }
     }
 
     SmartPointer(const SmartPointer& ptr) {
         if (this != &ptr) {
             this->_ptr = ptr._ptr;
             this->_count = ptr._count;
             (*this->_count)++;
         }
     }
 
     SmartPointer& operator=(const SmartPointer& ptr) {
         if (this->_ptr == ptr._ptr) {
             return *this;
         }
 
         if (this->_ptr) {
             (*this->_count)--;
             if (this->_count == 0) {
                 delete this->_ptr;
                 delete this->_count;
             }
         }
 
         this->_ptr = ptr._ptr;
         this->_count = ptr._count;
         (*this->_count)++;
         return *this;
     }
 
     T& operator*() {
         return *(this->_ptr);
 
     }
 
     T* operator->() {
         return this->_ptr;
     }
 
     ~SmartPointer() {
         (*this->_count)--;
		
         if (*this->_count == 0) {
             delete this->_ptr;
             delete this->_count;
         }
     }
 
     size_t use_count(){
         return *this->_count;
     }
 };

 //class Parent;
 class Child;
 
 class Parent {
 
 private:
	 SmartPointer<Child> child;
 public:
	 Parent() {
		 cout << "Parent构造函数" <<endl;
	 }
	 ~Parent() {
		 cout << "Parent析构函数" <<endl;
	 }
 
	 void setChild(SmartPointer<Child> s) {
		 child = s;
	 }
 };
 
 class Child {
 
 private:
	 SmartPointer<Parent> parent;
	 
 public:
	 Child() {
		 cout << "Child构造函数" <<endl;
	 }
	 ~Child() {
		 cout << "Child析构函数" <<endl;
	 }
	 void setParent(SmartPointer<Parent> s) {
		 parent = s;
	 }
 };
 
 void test5() {
 
	 {
		 SmartPointer<Parent> parent(new Parent());
		 SmartPointer<Child> child(new Child());
		 
		 parent->setChild(child);
		 child->setParent(parent);
	 
		 cout << "parent' s count" << parent.use_count() << endl;
		 cout << "child' s count" << child.use_count() << endl;
	 }

 	 /*
		child先出作用域,Child的引用计数减1,不为0,所以堆上的Child空间没有释放,且Child中持有Parent也没有机会被析构,Parent的引用计数也完全没有减少

		parent后出作用域,Parent的引用计数减1,不为0,所以堆上的Parent空间没有释放,且Parent中持有Child也没有机会被析构
 	*/
  
 }

int main() {

//	test1();
//	test2();
//	test3();
//	test4();
	test5();

	return 0;
}

