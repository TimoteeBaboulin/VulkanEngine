#include "TaskManager.h"
#include "Worker.h"

void Moonlit::Tasks::TaskManager::EnqueueTask(const Task& _task)
{
	Task taskCopy = _task;
	std::lock_guard<std::mutex> lock(m_taskMutex);
	m_taskQueue.push(taskCopy);

	m_taskCondition.notify_one();
}

void Moonlit::Tasks::TaskManager::WaitForAllTasks()
{
	m_taskMutex.lock();
	m_shutdown = true;
	m_taskMutex.unlock();

	m_taskCondition.notify_all();

	for (Worker& worker : m_workers)
	{
		worker.Join();
	}
}

size_t Moonlit::Tasks::TaskManager::GetWorkerCount()
{
	std::lock_guard<std::mutex> lock(m_workerMutex);
	return m_workers.size();
}

size_t Moonlit::Tasks::TaskManager::GetActiveWorkerCount()
{
	std::lock_guard<std::mutex> lock(m_workerMutex);
	return m_activeWorkerCount;
}
