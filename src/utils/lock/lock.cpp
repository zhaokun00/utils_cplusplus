#include <stdio.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

/*
	互斥量是一种同步原语,是一种线程同步的手段,用来保护多线程同一时间访问的共享数据

	std::mutex:独占的互斥量,不能递归使用
	std::timed_mutex:带超时的独占互斥量,不能递归使用
	std::recursive_mutex:递归互斥量,不带超时功能
	std::recursive_timed_mutex:带超时的递归互斥量

	这些互斥量的基本接口十分相近,都是通过lock()来阻塞线程,直到获得互斥量的所有权为止.在线程完成后就必须使用unlock()
	来解除对互斥量的占用,lock和unlock必须成对出现.try_lock()尝试锁定互斥量,成功返回true,失败返回false,是非阻塞的
*/

//测试:互斥锁
std::mutex g_test1_lock;

void test1_1() {

	g_test1_lock.lock(); //加锁

	std::cout << "start thread id = " << std::this_thread::get_id() << std::endl;

	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::cout << "end thread id = " << std::this_thread::get_id() << std::endl;
	
	g_test1_lock.unlock(); //解锁
}
void test1() {

	std::thread t1(test1_1);
    std::thread t2(test1_1);

	t1.join();
	t2.join();
}

//timed_mutex没有实验成功
std::timed_mutex g_test2_lock;

void test2_1() {

	std::chrono::seconds timeout(10);
	
	if(true == g_test2_lock.try_lock_for(timeout)) {
		std::cout << "start lock" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(20));
		g_test2_lock.unlock();
		std::cout << "end lock" << std::endl;
	}
	else {
		std::cout << "time out"<< std::endl;
	}

}

void test2() {

	std::thread t1(test2_1);	
    std::thread t2(test2_1);

	t1.join();
	t2.join();
}

/*测试lock_guard,lock_guard在构造时会自动锁定互斥量,而在退出作用域后进行析构是就会自动解锁
  lock_guard类是一个mutex的封装者,它为了拥有一个或多个mutex而提供了一种方便的RAII style机制,就是
  在资源获取的时候将其封装在某类的object中,利用栈资源会在相应的object的生命周期结束时自动销毁来释放
  资源

  std:lock_guard与std::unique_lock的区别,可以参考C++中的文档,看二者的区别

  std::unique_lock可以unlock,std:lock_guard不能

  互斥对象管理类模板的加锁策略
  前面提到std::lock_guard、std::unique_lock和std::shared_lock类模板在构造时是否加锁是可选的，C++11提供了3种加锁策略。

  策略	             tag type	                                 描述
 (默认)	                无	                          请求锁，阻塞当前线程直到成功获得锁。
 std::defer_lock	std::defer_lock_t	              不请求锁。
 std::try_to_lock	std::try_to_lock_t	              尝试请求锁，但不阻塞线程，锁不可用时也会立即返回。
 std::adopt_lock	std::adopt_lock_t	              假定当前线程已经获得互斥对象的所有权，所以不再请求锁。
	下表列出了互斥对象管理类模板对各策略的支持情况。

策略	                     std::lock_guard	std::unique_lock	std::shared_lock
(默认)	                        √	               √	               √(共享)
std::defer_lock	                ×	               √	               √
std::try_to_lock	            ×	               √	               √
std::adopt_lock	                √	               √	               √

下面的代码中std::unique_lock指定了std::defer_lock
std::mutex mt;
std::unique_lock<std::mutex> lck(mt, std::defer_lock);
assert(lck.owns_lock() == false);
lck.lock();
assert(lck.owns_lock() == true);
  
*/
std::mutex g_test3_lock;
void test3_1() {

	std::cout << "start thread id: " << std::this_thread::get_id() << std::endl;
	std::lock_guard<std::mutex> lock(g_test3_lock);
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::cout << "end thread id: " << std::this_thread::get_id() << std::endl;
}

void test3() {

	std::thread t1(test3_1);

	std::this_thread::sleep_for(std::chrono::seconds(1));
    std::thread t2(test3_1);

	t1.join();
	t2.join();
}

//测试条件变量
/*
	条件变量:
	条件变量阻塞一个或多个线程,直到收到另一个线程发出来的通知或者超时,才会唤醒当前阻塞的进程,条件变量
	需要和互斥量配合使用

	C++11提供了两种条件变量:
	1.std::condition_variable,配合std::unique_lock进行wait操作
	2.std::condition_variable_any和任意带有lock,unlock的mutex进行搭配的使用,比较灵活但效率略低

	条件变量使用过程如下:
	1.拥有条件变量的线程获取互斥锁
	2.循环检查某个条件,如果条件不满足,则阻塞直到条件满足,如果条件满足,则向下执行
	3.某个线程满足条件执行完之后调用notify_one或notify_all唤醒一个或者所有等待线程
*/

//定义条件变量
std::condition_variable_any g_condition;
//定义互斥锁
std::mutex g_test4_lock;

//等待信号,阻塞当前线程
void test_wait() {

	while(1) {
		std::unique_lock<std::mutex> lock(g_test4_lock);
		std::cout << "wait start..." << std::endl;
		
		g_condition.wait(lock); //wait函数会释放当前的lock锁
		
		std::cout << "wait end..." << std::endl;
	}
}

//发送信号量
void test_post() {

	std::this_thread::sleep_for(std::chrono::seconds(5));
	while(1) {

		std::cout << "post start..." << std::endl;

		g_condition.notify_all();

		std::this_thread::sleep_for(std::chrono::seconds(5));
		std::cout << "post end..." << std::endl;
	}
}

void test4() {

	std::thread t1(test_wait);
	std::thread t2(test_wait);
	std::thread t3(test_wait);

	std::thread t(test_post);

	t1.join();
	t2.join();
	t3.join();



//	t.join();
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
	return 0;
}

