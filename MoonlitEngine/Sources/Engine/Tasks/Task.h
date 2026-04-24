#pragma once

#include <functional>
#include <memory>
#include <mutex>

namespace Moonlit::Tasks
{
	class Task
	{
	public:
		Task() = delete;
		Task(std::function<void()> task, std::vector<std::shared_ptr<Task>> dependencies = {});
		~Task();

		void addDependency(std::shared_ptr<Task> task);
		void addDependencies(std::vector<std::shared_ptr<Task>> tasks);

		void run();
		bool canRun();
		[[nodiscard]] bool isCompleted() const { return m_isCompleted; }

	private:
		std::vector<std::shared_ptr<Task>> m_dependencies;

		std::mutex m_mutex;

		std::function<void()> m_taskFunction;
		bool m_isCompleted;
	};
}