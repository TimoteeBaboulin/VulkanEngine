#include "WorkerManager.h"

#include <stdexcept>

#include "Debug/Logger.h"
#include "Task.h"

namespace Moonlit::Tasks
{
    WorkerManager::WorkerManager()
    {
        MainWorkerManager = this;
        CurrentWorkerManager = this;
        ParentWorkerManager = nullptr;

        m_threadCount = calculateThreadCount();
        generateThreads();

        LOG_INFO("Initialising main worker manager with " + std::to_string(m_threadCount) + " threads");
    }

    WorkerManager::WorkerManager(WorkerManager *_parent, int _threadCount)
    {
        if (_threadCount <= 0)
        {
            throw std::runtime_error("Thread count must be greater than zero");
        }

        m_threadCount = _threadCount;
        ResultType result = _parent->tryAcquire(m_threadCount, m_threads, -1.0f);
        if (result == ResultType::IMPOSSIBLE_REQUEST)
        {
            throw std::runtime_error("Impossible request");
        }
    }

    ResultType WorkerManager::tryAcquire(int count, std::vector<std::thread> &_outThreads, float _timeOut) {
        return ResultType::IMPOSSIBLE_REQUEST;
    }

    std::shared_ptr<Task> WorkerManager::acquireTask()
    {
        std::shared_ptr<Task> task = nullptr;

        auto it = std::find_if(m_tasks.begin(), m_tasks.end(), [](auto& t) {
            return t->canRun();
        });
        if (it != m_tasks.end())
        {
            task = (*it);
            m_tasks.erase(it);
        }

        return task;
    }

    void WorkerManager::addTask(TASK_FUNC _task)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        {
            m_tasks.emplace_back(std::make_shared<Task>(_task));
        }

        m_cv.notify_one();
    }

    void WorkerManager::addTask(std::shared_ptr<Task> _task)
    {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_tasks.emplace_back(_task);
        }

        m_cv.notify_one();
    }

    void WorkerManager::addTasks(std::vector<TASK_FUNC>& _tasks)
    {
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            for (auto& task : _tasks)
            {
                std::shared_ptr<Task> newTask = std::make_shared<Task>(task);
                m_tasks.emplace_back(newTask);
            }
        }

        m_cv.notify_all();
    }

    void WorkerManager::addTasks(std::vector<std::shared_ptr<Task>> _tasks)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        for (auto& task : _tasks)
        {
            m_tasks.emplace_back(task);
        }

        m_cv.notify_all();
    }

    void WorkerManager::drain()
    {
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            m_state = State::DRAINING;
            m_drainingCv.wait(lock, [this]() {
                return m_tasks.empty();
            });

            m_state = State::STOPPED;
        }
    }

    int WorkerManager::calculateThreadCount()
    {
        int concurrency = std::thread::hardware_concurrency();
        if (concurrency == 0)
        {
            LOG_WARNING("No information on hardware concurrency, choosing backup value");
            return MIN_WORKER_THREADS;
        }

        if (concurrency < MIN_WORKER_THREADS)
        {
            if (concurrency == 1)
            {
                LOG_ERROR("Hardware concurrence is a single thread");
                throw std::runtime_error("Hardware concurrency is a single thread");
            }

            LOG_WARNING("Limited concurrency is less than minimum worker threads");
            return concurrency - 1;
        }

        if (concurrency - OS_THREAD_WORKER_HEADSPACE < MIN_WORKER_THREADS)
        {
            if (concurrency == MIN_WORKER_THREADS)
            {
                return MIN_WORKER_THREADS - 1;
            }

            return MIN_WORKER_THREADS;
        }

        return concurrency - OS_THREAD_WORKER_HEADSPACE;
    }

    void WorkerManager::generateThreads()
    {
        m_threads.reserve(m_threadCount);

        for (int i = 0; i < m_threadCount; i++)
        {
            m_threads.push_back(std::thread(threadLoop, MainWorkerManager, ParentWorkerManager, this));
        }
    }

    void threadLoop(WorkerManager *_main, WorkerManager *_parent, WorkerManager *_current)
    {
        MainWorkerManager = _main;
        ParentWorkerManager = _parent;
        CurrentWorkerManager = _current;

        WorkerManager& manager = *_current;

        auto checkFunc = [&]() {
            return manager.IsShuttingDown() || !manager.m_tasks.empty();
        };

        while (true)
        {
            std::shared_ptr<Task> task = nullptr;

            {
                std::unique_lock<std::mutex> lock(manager.m_mutex);
                manager.m_cv.wait(lock, checkFunc);

                if (manager.IsShuttingDown() && manager.m_tasks.empty())
                {
                    return;
                }

                task = manager.acquireTask();
            }

            if (task)
            {
                if (task->canRun())
                {
                    LOG_INFO("Running a task");
                    task->run();

                    bool isDraining = false;
                    {
                        std::unique_lock<std::mutex> lock(manager.m_mutex);
                        isDraining = manager.m_state == WorkerManager::DRAINING;
                    }

                    if (isDraining)
                    {
                        manager.m_drainingCv.notify_all();
                    }
                }
                else
                {
                    manager.addTask(task);
                }
            }
        }
    }
}
