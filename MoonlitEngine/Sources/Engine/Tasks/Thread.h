#pragma once
#include <thread>

namespace Moonlit::Tasks
{
    class WorkerManager;
    class Thread {
    public:
        Thread(WorkerManager* _manager, std::thread& _thread);
    private:
        std::thread& m_thread;
    };
}


