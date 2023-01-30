
#include <iostream>

#include "thread_pool.hpp"

class copyonly_object
{
   public:
    copyonly_object(int i) : data(i){};
    copyonly_object() = default;
    copyonly_object(copyonly_object&& other) = delete;
    copyonly_object& operator=(copyonly_object&& other) = delete;
    copyonly_object(const copyonly_object& other) : data(other.data){};
    copyonly_object(copyonly_object& other) : data(other.data){};
    copyonly_object& operator=(const copyonly_object& other)
    {
        data = other.data;
        return *this;
    };
    int data;
};

class noncopyable_object
{
   public:
    noncopyable_object(int i) : data(std::make_unique<int>(i)){};
    noncopyable_object() = default;
    noncopyable_object(noncopyable_object&& other) : data(std::move(other.data)){};
    noncopyable_object& operator=(noncopyable_object&& other)
    {
        data = std::move(other.data);
        return *this;
    }
    noncopyable_object(const noncopyable_object&) = delete;
    noncopyable_object(noncopyable_object&) = delete;
    noncopyable_object& operator=(const noncopyable_object&) = delete;
    std::unique_ptr<int> data;
};

int copyF(copyonly_object a, copyonly_object b) { return a.data + b.data; }
int moveF(noncopyable_object& a, noncopyable_object& b) { return *a.data + *b.data; }

int mixF(copyonly_object a, noncopyable_object& b) { return a.data + *b.data; }
int simpleMixF(int a, noncopyable_object& b) { return a + *b.data; }
int main()
{
    ThreadPool tp(1);
    // 通过
    {
        // auto ret = tp.enqueue(copyF, 1, 2);
        // std::cout << "copy:" << ret.get() << std::endl;
    }
    // 通过
    {
        // noncopyable_object a(3);
        // noncopyable_object b(4);
        // auto ret = tp.enqueue(moveF, a,b);
        // std::cout << "move:" << ret.get() << std::endl;
    }  
    // 未通过, 无法推导ReturnType的参数
    {
        copyonly_object a(7);
        noncopyable_object b(8);
        auto ret = tp.enqueue(simpleMixF, 7, b);
        std::cout << ret.get() << std::endl;
    } 

    // 通过
    {
        // copyonly_object a(5);
        // noncopyable_object b(6);
        // auto task = std::bind(mixF, std::forward<copyonly_object>(a), std::forward<noncopyable_object>(b));
        // int i = task();
        // std::cout << "mix:" << i << std::endl;
    }
    // 未通过
    {  
        // copyonly_object a(5);
        // noncopyable_object b(6);
        // auto task = std::bind(mixF, std::forward<copyonly_object>(a), b);
        // int i = task();
        // std::cout << "mix:" << i << std::endl;
    }
    return 0;
}