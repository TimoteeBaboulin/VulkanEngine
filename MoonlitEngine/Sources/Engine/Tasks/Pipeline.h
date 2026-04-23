#pragma once

namespace Moonlit::Tasks
{
    class WorkerManager;

    class Pipeline {
    public:
        Pipeline(WorkerManager* _manager);

    private:
        WorkerManager* m_manager;
    };

}

