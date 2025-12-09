#include "pch.h"

#include "Engine/Tasks/TaskManager.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(TaskManager, Initialisation) {
	MoonlitEngine::Tasks::TaskManager& manager = MoonlitEngine::Tasks::TaskManager::Instance();
	EXPECT_EQ(manager.GetWorkerCount(), 8);
	EXPECT_EQ(manager.GetActiveWorkerCount(), 0);
}

TEST(TaskManager, ProcessTasks)
{
	MoonlitEngine::Tasks::TaskManager& manager = MoonlitEngine::Tasks::TaskManager::Instance();
	std::atomic<int> counter = 0;
	const int taskCount = 100;
	for (int i = 0; i < taskCount; i++)
	{
		MoonlitEngine::Tasks::Task task;
		task.taskFunction = [&counter]() {
			counter++;
		};
		manager.EnqueueTask(task);
	}
	manager.WaitForAllTasks();
	EXPECT_EQ(counter.load(), taskCount);
	EXPECT_EQ(manager.GetActiveWorkerCount(), 0);
}