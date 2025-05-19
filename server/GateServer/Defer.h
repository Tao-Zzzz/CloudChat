#pragma once

#include <functional>

//RAII˼��
class Defer{
public:
	// ����һ��lambda���ʽ����ָ��
	Defer(std::function<void()> func) : func_(func) {}

	// ������ʱִ�д���ĺ���
	~Defer() {
		func_();
	}
private:
	std::function<void()> func_;
};

