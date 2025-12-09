#pragma once

#include <mutex>
#include <queue>

#include "Worker.h"
#include "Task.h"

#include "MoonlitExport.h"

namespace MoonlitEngine
{
	namespace Tasks
	{
		constexpr size_t WORKER_COUNT = 8;

		class MOONLIT_API TaskManager
		{
			friend class Worker;

		public:
			static TaskManager& Instance()
			{
				static TaskManager instance;
				return instance;
			}

			void EnqueueTask(const Task& _task);

			void WaitForAllTasks();
			size_t GetWorkerCount();
			size_t GetActiveWorkerCount();

		private:
			// Mutex used to protect task queue
			std::mutex m_taskMutex;
			// Mutex used to protect worker thread management
			std::mutex m_workerMutex;

			std::vector<Worker> m_workers;
			std::queue<Task> m_taskQueue;
			size_t m_activeWorkerCount = 0;

			std::condition_variable m_taskCondition;

			bool m_shutdown = false;
		};
	}
}