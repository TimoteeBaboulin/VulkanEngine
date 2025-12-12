#include "Engine/Tasks/Worker.h"

#include <mutex>
#include <condition_variable>

#include "Engine/Tasks/TaskManager.h"

MoonlitEngine::Tasks::Worker::Worker()
	: m_thread([this]() { this->Run(); })
{
}

bool MoonlitEngine::Tasks::Worker::TryGetTask()
{
	TaskManager& manager = TaskManager::Instance();

	// Must return true for the condition variable to wake up
	if (manager.m_shutdown)
		return true;

	if (manager.m_taskQueue.empty())
		return false;
	
	m_task = manager.m_taskQueue.front();
	manager.m_taskQueue.pop();
	return true;
}

void MoonlitEngine::Tasks::Worker::UpdateWorkerCount(TaskManager& _manager, bool _increment)
{
	_manager.m_workerMutex.lock();
	if (_increment)
		_manager.m_activeWorkerCount++;
	else
		_manager.m_activeWorkerCount--;
	_manager.m_workerMutex.unlock();
}

void MoonlitEngine::Tasks::Worker::Run()
{
	TaskManager& taskManager = TaskManager::Instance();

	while (true)
	{
		// Use a condition variable to wait for tasks or the shutdown signal
		std::unique_lock<std::mutex> lock(
			taskManager.m_taskMutex);
		taskManager.m_taskCondition.wait(lock, [this]() {return this->TryGetTask(); });

		// Wake up condition
		// The lock will unlock when going out of scope
		if (taskManager.m_shutdown)
			return;

		lock.unlock();

		// Execute the task
		UpdateWorkerCount(taskManager, true);

		m_task.taskFunction();

		UpdateWorkerCount(taskManager, false);
	}
}
