# Thread Pool 线程池

基于C++标准库，用于探索实现C++线程池。从最简单的线程池到复杂的线程池。

## 1. 编译

为了能够使用到`C++23`新特性，请使用Visual Studio 2022和GCC 12作为编译器。

+ windows:  

```powershell
    mkdir build
    cd build
    cmake -G "Visual Studio 17 2022" ..
    cmake --build . --config Release 
```

+ linux:

```shell
    mkdir build && cd build
    cmake ..
    make
```
