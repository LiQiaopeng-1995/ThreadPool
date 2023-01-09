#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>
#include <iostream>

class ThreadPool
{
    // 线程池终止标志位
    bool stop = false;  
    // 任务队列
    std::queue<std::move_only_function<void()> > work_queue;
    // 线程容器
    std::vector<std::thread> threads;

    // 同步
    std::mutex queue_mutex;
    std::condition_variable condition;

    void woker_thread()
    {
        // 只要线程池没有终止, 工作线程就反复从任务队列取任务, 并执行任务,
        int i = 0;
        while (1)
        {   
            std::cout << "tid: " << std::this_thread::get_id() << " work " << i << " times." << std::endl;
            std::function<void()> task;
            std::unique_lock<std::mutex> lock(this->queue_mutex);
            if (!this->work_queue.empty())
            {
                // 取任务的时候需要加锁, 防止多个线程同时取任务
                task = std::move(this->work_queue.front());
                this->work_queue.pop();
                task();
                i++;
            }
            else if (this->stop)
            {
                break;
            }
            else
            {
                condition.wait(lock);
            }
        }
    }

   public:
    ThreadPool(size_t thread_count)
    {
        try
        {   
            // 初始化线程数为thread_count的线程池
            stop = false;
            for (unsigned i = 0; i < thread_count; ++i)
            {
                threads.push_back(std::thread(&ThreadPool::woker_thread, this));
            }
        }
        catch (...)
        {   
            std::cout << "create ThreadPool Fail." << std::endl;
            stop = true;
            throw;
        }
    }

    ~ThreadPool()
    {   
        std::cout << std::this_thread::get_id() << ": ThreadPool deconstruct." << std::endl;
        {   
            // 加锁防止有工作线程取任务
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        // 通知所有线程继续执行
        condition.notify_all();
        // 执行完所有线程
        for (std::thread &t : threads) 
        {   
            if (t.joinable()) t.join();
        }
    }

    template <typename FunctionType>
    void submit(FunctionType f)
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            if (stop) 
            {   
                std::cout << "enqueque on stopped ThreadPool." << std::endl;
                throw std::runtime_error("enqueque on stopped ThreadPool");
            }
            work_queue.push(std::function<void()>(f));
        }
        this->condition.notify_one();
    }
};

#endif 