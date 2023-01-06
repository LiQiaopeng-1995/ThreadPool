#include "simple_thread_pool.hpp"

void f()
{
    std::cout << "tid: " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100000));
    std::cout << "tid: " << std::this_thread::get_id() << std::endl;
}
int main()
{
    ThreadPool thread_pool(4);
    const auto thread_count = std::thread::hardware_concurrency();
    for (int i = 0; i < thread_count; i++)
    {
        thread_pool.submit<std::function<void()>>(f);
    }
}