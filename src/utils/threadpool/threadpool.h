#ifndef THREAD_m_pool_H
#define THREAD_m_pool_H

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

#define  MAX_THREAD_NUM 256

class ThreadPool
{
    using Task = std::function<void()>;
   
    std::vector<std::thread> m_pool; //线程池
    std::queue<Task> m_tasks; //任务队列
    std::mutex m_lock; //同步锁
    std::condition_variable m_cv_task; //条件阻塞
    std::atomic<bool> m_stoped; //是否关闭提交
    std::atomic<int>  m_idlThrNum; //空闲线程数量

public:
    inline ThreadPool(unsigned short size = 4) :m_stoped{ false }
    {
        m_idlThrNum = size < 1 ? 1 : size;

        for (size = 0; size < m_idlThrNum; ++size)
        {   //初始化线程数量
            m_pool.emplace_back(
                [this]
                { 
                    while(!this->m_stoped) //工作线程函数
                    {
                        std::function<void()> task;
						std::cout << "1111" << std::endl;
                        {   
                            std::unique_lock<std::mutex> lock(this->m_lock);
							/*
							带条件的被阻塞：wait函数设置了谓词(Predicate)，只有当pred条件为false时调用该wait函数才会阻塞当前线程，
							并且在收到其它线程的通知后只有当pred为true时才会被解除阻塞
							*/
                            this->m_cv_task.wait(lock,[this] {
					        	return this->m_stoped.load() || !this->m_tasks.empty();
					            }
                            );
                            if (this->m_stoped && this->m_tasks.empty())
                                return;
                            task = std::move(this->m_tasks.front()); // 取一个 task
                            this->m_tasks.pop();
                        }
                        m_idlThrNum--;
                        task();
                        m_idlThrNum++;
                    }
                }
            );
        }
    }

    inline ~ThreadPool()
    {
        m_stoped.store(true);
        m_cv_task.notify_all();
        for (std::thread& thread : m_pool) {
            //thread.detach(); // 让线程“自生自灭”
            if(thread.joinable())
                thread.join(); // 等待任务结束， 前提：线程一定会执行完
        }
    }

public:
    // 提交一个任务
    // 调用.get()获取返回值会等待任务执行完,获取返回值
    // 有两种方法可以实现调用类成员，
    // 一种是使用   bind： .commit(std::bind(&Dog::sayHello, &dog));
    // 一种是用 mem_fn： .commit(std::mem_fn(&Dog::sayHello), &dog)
    template<class F, class... Args>
    auto commit(F&& f, Args&&... args) ->std::future<decltype(f(args...))> {

        if (m_stoped.load()) {
			std::cout << "commit on ThreadPool is stopped" << std::endl;
		}
 
        using RetType = decltype(f(args...)); // typename std::result_of<F(Args...)>::type, 函数 f 的返回值类型

        auto task = std::make_shared<std::packaged_task<RetType()> >(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        std::future<RetType> future = task->get_future();

		//添加任务到队列
        {  
            std::lock_guard<std::mutex> lock(m_lock);
            m_tasks.emplace(

                [task]()
                {
                    (*task)();
                }
            );
        }
        m_cv_task.notify_one(); // 唤醒一个线程执行

        return future;
    }

    //空闲线程数量
    int idlCount() { return m_idlThrNum; }

};

#endif
