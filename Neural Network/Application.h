#pragma once

#include <iostream>
#include <thread>
#include <condition_variable>
#include <functional>

class Application
{
private:
	bool m_running;
	bool m_working;
	bool m_work;
	std::function<void()> m_whileFunc;
	std::condition_variable m_condition;
	std::mutex m_mutex;

public:
	Application(const std::function<void()>& func);

	void Init();
	void Execute(const std::function<void()>& func);
	void End();

	~Application();
};

