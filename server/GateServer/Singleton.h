#pragma once
#include <iostream>
#include <mutex>

template <typename T>
class Singleton {
protected:
    Singleton() = default;
    // 不允许拷贝构造和拷贝赋值
    Singleton(const Singleton<T>&) = delete;
    Singleton& operator = (const Singleton<T>& st) = delete;

    // 智能指针, 静态（一个实例）
    static std::shared_ptr<T> _instance;

public:
    static std::shared_ptr<T> GetInstance() {

        // 只初始化一次
        static std::once_flag s_flag;
        // s_flag会置为true
        std::call_once(s_flag, [&]() {
            // make_shared 需要构造函数,而这里的构造函数私有
            _instance = std::shared_ptr<T>(new T);

            });

        return _instance;
    }

    void PrintAddress() {
        std::cout << _instance.get() << std::endl;
    }

    ~Singleton() {
        std::cout << "this is singleton destruct" << std::endl;
    }
};

template<typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;
