#include "simple_thread_pool.hpp"

void f()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

}
int main()
{
    ThreadPool thread_pool(2);
    const auto thread_count = std::thread::hardware_concurrency();
    for (int i = 0; i < thread_count; i++)
    {
        thread_pool.submit<std::function<void()>>(f);
    }
    thread_pool.~ThreadPool();
    return 0;
}