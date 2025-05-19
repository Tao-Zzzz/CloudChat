#pragma once
#include <iostream>
#include <mutex>

template <typename T>
class Singleton {
protected:
    Singleton() = default;
    // ������������Ϳ�����ֵ
    Singleton(const Singleton<T>&) = delete;
    Singleton& operator = (const Singleton<T>& st) = delete;

    // ����ָ��, ��̬��һ��ʵ����
    static std::shared_ptr<T> _instance;

public:
    static std::shared_ptr<T> GetInstance() {

        // ֻ��ʼ��һ��
        static std::once_flag s_flag;
        // s_flag����Ϊtrue
        std::call_once(s_flag, [&]() {
            // make_shared ��Ҫ���캯��,������Ĺ��캯��˽��
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
