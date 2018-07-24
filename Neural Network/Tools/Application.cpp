#include "Application.h"

Application::Application(const std::function<void()>& func)
	: m_running(true)
	, m_work(true)
{
	m_whileFunc = func;
}

void Application::Init()
{
	while (m_running)
	{
		if (m_work)
		{
			m_working = true;

			m_whileFunc();

			m_working = false;
		}

		m_condition.notify_all();
	}
}

void Application::Execute(const std::function<void()>& func)
{
	m_work = false;

	std::unique_lock<std::mutex> lock(m_mutex);

	while (m_working) { m_condition.wait(lock); }

	func();

	m_work = true;
}

void Application::End()
{
	m_running = false;
}

Application::~Application()
{
}
