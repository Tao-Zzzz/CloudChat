#pragma once

#include <functional>

//RAII思想
class Defer{
public:
	// 接收一个lambda表达式或函数指针
	Defer(std::function<void()> func) : func_(func) {}

	// 在析构时执行传入的函数
	~Defer() {
		func_();
	}
private:
	std::function<void()> func_;
};

