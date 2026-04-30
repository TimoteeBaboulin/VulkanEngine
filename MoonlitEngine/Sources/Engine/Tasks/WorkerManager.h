#pragma once
#include <condition_variable>
#include <deque>
#include <thread>
#include <vector>
#include <functional>

#include "Task.h"

constexpr int OS_THREAD_WORKER_HEADSPACE = 3;
constexpr int MIN_WORKER_THREADS = 4;
constexpr float MAX_WAIT_THRESHOLD = 60; // In seconds

namespace Moonlit::Tasks
{
    enum ResultType {
        SUCCESS = 0,
        TIMEOUT = 1,
        IMPOSSIBLE_REQUEST = 2
    };

    using TASK_FUNC = std::function<void()>;

    class WorkerManager;
    class Pipeline;

    void threadLoop(WorkerManager* _main, WorkerManager* _parent, WorkerManager* _current);

    class WorkerManager
    {
    public:
        enum State {
            RUNNING,
            DRAINING,
            STOPPED
        };

        WorkerManager();
        WorkerManager(WorkerManager* _parent, int _threadCount);

        ResultType tryAcquire(int count, std::vector<std::thread>& _outThreads, float _timeOut);
        // release(int count, std::vector<std::thread>& _threads);
        // release(WorkerManager* _manager);
        //
        // ResultType createPipeline(int _threadCount);

        bool IsShuttingDown() const {return m_isShuttingDown;}
        std::shared_ptr<Task> acquireTask();
        void addTask(TASK_FUNC _task);
        void addTask(std::shared_ptr<Task> _task);
        void addTasks(std::vector<TASK_FUNC>& _tasks);
        void addTasks(std::vector<std::shared_ptr<Task>> _tasks);

        void drain();

    protected:
        int m_threadCount = 0;
        std::vector<std::thread> m_threads;
        std::deque<std::shared_ptr<Task>> m_tasks;

        std::condition_variable m_cv;
        std::condition_variable m_drainingCv;
        std::atomic<State> m_state = RUNNING;
        std::atomic<int> m_runningTaskCount;
        std::mutex m_mutex;

        bool m_isShuttingDown = false;

        int calculateThreadCount();
        void generateThreads();

        friend void threadLoop(WorkerManager* _main, WorkerManager* _parent, WorkerManager* _current);
    };

    inline thread_local WorkerManager* MainWorkerManager = nullptr;
    inline thread_local WorkerManager* CurrentWorkerManager = nullptr;
    inline thread_local WorkerManager* ParentWorkerManager = nullptr;
}
