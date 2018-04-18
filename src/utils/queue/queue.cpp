#include <stdio.h>
#include <iostream>
#include <queue>
#include <chrono>

//using namespace std;

/*
	queue模板类的定义在<queue>头文件中,与stack模板类很相似,queue模板类也需要两个模板参数,一个是元素类型,一个容器类型
	元素类型时必要的,容器类型时可选的,默认为deque类型

	queue的基本操作有:

	入队，如例：q.push(x); 将x 接到队列的末端。
	出队，如例：q.pop(); 弹出队列的第一个元素，注意，并不会返回被弹出元素的值。
	访问队首元素，如例：q.front()，即最早被压入队列的元素。
	访问队尾元素，如例：q.back()，即最后被压入队列的元素。
	判断队列空，如例：q.empty()，当队列空时，返回true。
	访问队列中的元素个数，如例：q.size()
*/

//测试:普通的队列-queue
void test1() {

	std::queue<std::string> q;

	q.push("zhao"); //入队
	q.push("qian");
	q.push("sun");
	q.push("li");

	std::cout << "size = " << q.size() << std::endl;
	std::cout << "front = " << q.front() << std::endl; //返回队列的首元素
	std::cout << "end = " << q.back() << std::endl; //返回队列的尾元素
	
	q.pop(); //出队

	std::cout << "size = " << q.size() << std::endl;
	std::cout << "front = " << q.front() << std::endl; 
	std::cout << "end = " << q.back() << std::endl;
}

/*
	priority_queue优先级队列,其底层是用堆来实现的,在优先级队列中,队首元素一定是当前队列中优先级最高的那一个。
	在优先级队列中,没有front()函数与back()函数,而只能通过top()函数来访问队首元素,也就是优先级最高的元素

	1.基本类型数据的优先级设置
	  此处指的基本数据类型就是int型、double型、char型等可以直接使用的数据类型,优先级队列对他们的优先级设置一般是
	  数字越大的优先级越高,因此队首元素就是优先队列内元素最大的那个(如果是char型,则是字典序中最大的那个)

	   //下面两种优先队列的定义是等价的
    	priority_queue<int> q;
    	priority_queue<int,vector<int>,less<int> >;//后面有一个空格

    	其中第二个参数(vector)是来承载底层数据结构堆的容器,第三个参数(less),则是一个比较类,less表示数字越大优先级越高,
    	而greater表示数字小优先级越高,如果想让优先队列总是把最小的元素放在队首,只需要进行如下定义:

    	priority_queue<int,vector<int>,greater<int> >q;
*/
//测试具有优先级的队列
void test2() {

	std::priority_queue<int> rq; //默认是数字越大,优先级越高

	rq.push(1); //存储数据
	rq.push(2); 

	for(int i = 0;i < 2;i++) {
		std::cout << "data = " << rq.top() << std::endl; //访问首端元素,输出数据为2,1
		rq.pop(); //弹出数据
	}
	
}

//测试数字小的优先级越高
void test3() {

	std::priority_queue<int,std::vector<int>,std::greater<int> > rq;

	rq.push(1); //存储数据
	rq.push(2); 

	for(int i = 0;i < 2;i++) {
		std::cout << "data = " << rq.top() << std::endl; //访问首端元素,输出数据为1,2
		rq.pop(); //弹出数据
	}
}

//测试自定义结构体
struct Human {

	int grade;

};

//第一种方式
#if 1
struct cmp{  

    bool operator()(Human a, Human b) {

//		return a.grade > b.grade; //值越小优先级越高
        return a.grade < b.grade; //值越大优先级越高  
    }  
};
#endif

void test4() {

	std::priority_queue<Human,std::vector<Human>, cmp > rq;

	struct Human h1;
	struct Human h2;

	h1.grade = 10;
	h2.grade = 20;

	rq.push(h1);
	rq.push(h2);

	for(int i = 0;i < 2;i++) {
		std::cout << "data = " << rq.top().grade << std::endl;
		rq.pop(); //弹出数据
	}
	
}

struct Student {

	int grade;
//第二种方式
	friend bool operator <(Student s1,Student s2) {

		return s1.grade > s2.grade; //值越小优先级越高
//		return s1.grade < s2.grade; //值越大优先级越高
	}

};


void test5() {

	std::priority_queue<Student> rq;

	struct Student h1;
	struct Student h2;

	h1.grade = 10;
	h2.grade = 20;

	rq.push(h1);
	rq.push(h2);

	for(int i = 0;i < 2;i++) {
		std::cout << "data = " << rq.top().grade << std::endl;
		rq.pop(); //弹出数据
	}
}

class Teacher {

	public:
		int grade;

		friend bool operator <(Teacher s1,Teacher s2) {
			return s1.grade > s2.grade;
			//return s1.grade < s2.grade;
		}
};

void test6() {

	std::priority_queue<Teacher> rq;

	Teacher h1;
	Teacher h2;

	h1.grade = 10;
	h2.grade = 20;

	rq.push(h1);
	rq.push(h2);

	for(int i = 0;i < 2;i++) {
		std::cout << "data = " << rq.top().grade << std::endl;
		rq.pop(); //弹出数据
	}
}

int main() {

//	test1();
//	test2();
//	test3();
//	test4();
//	test5();
	test6();

	return 0;
}

