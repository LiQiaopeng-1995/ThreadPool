# 返回任务结果的线程池

如何解决`std::package_task<>`的MoveOnly和`std::function<>`的CopyOnly，

## 1. 测试progschj_threadpool能否支持MoveOnly的函数对象

对`test_bind.cpp`进行编译  

## 2. 测试`std::move_only_function`

编译`sample.cpp`，并运行
