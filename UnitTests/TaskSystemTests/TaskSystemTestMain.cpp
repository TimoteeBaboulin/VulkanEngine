#include <gtest/gtest.h>

#include "Debug/Logger.h"
#include "Engine/Tasks/WorkerManager.h"

namespace Moonlit::Tasks
{
    TEST(TaskSystem, SimpleDependencies)
    {
        auto* manager = new WorkerManager();
        std::vector<std::shared_ptr<Task>> tasks;
        const size_t taskCount = 20;

        for (size_t i = 0; i < taskCount; i++)
        {
            std::shared_ptr<Task> mainTask = std::make_shared<Task>([i]() {
                LOG_INFO("Task " + std::to_string(i) + " completed");
            });
            std::shared_ptr<Task> dependantTask = std::make_shared<Task>([i]() {
                LOG_INFO("Dependant task " + std::to_string(i) + " completed");
            });
            dependantTask->addDependency(mainTask);

            tasks.push_back(mainTask);
            tasks.push_back(dependantTask);
        }

        manager->addTasks(tasks);

        manager->drain();
    }
}