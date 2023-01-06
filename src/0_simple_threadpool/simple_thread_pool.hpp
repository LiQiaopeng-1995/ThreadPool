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
    bool stop = false;  // 线程池终止标志位
    // 任务队列
    std::queue<std::function<void()> > work_queue;
    // 线程容器
    std::vector<std::thread> threads;

    // 同步
    std::mutex queue_mutex;
    std::condition_variable condition;

    void woker_thread()
    {
        // 只要线程池没有终止, 工作线程就反复从任务队列取任务, 并执行任务,
        std::cout << "worker thread." << std::endl;
        std::cout << stop << std::endl;
        while (!stop)
        {   
            std::cout << "worker thread." << std::endl;
            std::function<void()> task;
            std::unique_lock<std::mutex> lock(this->queue_mutex);
            if (!this->work_queue.empty())
            {
                // 取任务的时候需要加锁, 防止多个线程同时取任务
                task = std::move(this->work_queue.front());
                this->work_queue.pop();
                task();
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
            for (unsigned i = 0; i < thread_count; ++i)
            {
                threads.push_back(std::thread(&ThreadPool::woker_thread, this));
            }
        }
        catch (...)
        {
            stop = true;
            throw;
        }
    }

    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
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
            if (stop) throw std::runtime_error("enqueque on stopped ThreadPool");
            work_queue.push(std::function<void()>(f));
        }
        this->condition.notify_one();
    }
};

#endif 