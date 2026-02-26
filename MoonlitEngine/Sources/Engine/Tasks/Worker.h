#pragma once

#include <thread>

#include "Task.h"

namespace Moonlit::Tasks
{
	class Worker
	{
	public:
		Worker();

		bool TryGetTask();
		void UpdateWorkerCount(class TaskManager& _manager, bool _increment);
		void Run();

		void Join()
		{
			if (m_thread.joinable())
			{
				m_thread.join();
			}
		}

	private:
		std::thread m_thread;
		bool m_running = false;

		struct Task m_task;
	};
}