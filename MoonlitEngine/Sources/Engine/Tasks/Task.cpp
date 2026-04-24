#include "Task.h"

#include <memory>

#include "Debug/Logger.h"

namespace Moonlit::Tasks
{
    Task::Task(std::function<void()> task, std::vector<std::shared_ptr<Task>> dependencies)
    {
        if (!dependencies.empty())
        {
            m_dependencies = dependencies;
        }

        m_taskFunction = task;
        m_isCompleted = false;
    }

    Task::~Task()
    {
        LOG_INFO("Deleting task");
    }

    void Task::addDependency(std::shared_ptr<Task> task)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_dependencies.emplace_back(task);
    }

    void Task::addDependencies(std::vector<std::shared_ptr<Task>> tasks)
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        for (auto& task : tasks)
        {
            m_dependencies.emplace_back(task);
        }
    }

    void Task::run()
    {
        if (!canRun())
        {
            return;
        }

        m_taskFunction();

        m_isCompleted = true;
    }

    bool Task::canRun()
    {
        if (m_isCompleted)
        {
            return false;
        }

        {
            std::unique_lock<std::mutex> lock(m_mutex);

            std::erase_if(m_dependencies, [](std::shared_ptr<Task> task){ return task->isCompleted();});
            return m_dependencies.empty();
        }
    }
}
