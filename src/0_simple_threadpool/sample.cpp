#include "simple_thread_pool.hpp"

void f()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
int main()
{   
    ThreadPool thread_pool(2);
    std::cout << "create thread pool." << std::endl;
    for (int i = 0; i < 3; i++)
    {
        thread_pool.submit<std::function<void()>>(f);
    }
    std::cout << "submit task." << std::endl;
    return 0;
}