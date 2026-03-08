#pragma once

#include <functional>
#include <map>
#include <memory>

#include "Debug/Logger.h"
#include "MoonlitExport.h"

namespace Moonlit::Events
{
	struct EventEntryHandle {
		size_t id = -1;
		bool isValid;

		bool valid() {
			return isValid && id != -1;
		}
	};

	template<typename ReturnT, typename... paramTs>
	class Event
	{
	public:
		using EventFunctionT = std::function<ReturnT(paramTs...)>;

		Event()
		{
			m_ownerHandle = 0;
			m_nextHandleId = 1;
		}

		Event(void* const _owner)
		{
			m_ownerHandle = reinterpret_cast<uintptr_t>(_owner);
			m_nextHandleId = 1;
		}

		~Event() {
			for (auto& [key, func] : m_eventFunctions)
			{
				key->isValid = false;
			}

			m_eventFunctions.clear();
		}

		std::shared_ptr<EventEntryHandle> operator+=(EventFunctionT _func)
		{
			return Subscribe(_func);
		}

		void operator-=(std::shared_ptr<EventEntryHandle> _handle)
		{
			if (!_handle->valid()) {
				return;
			}

			Unsubscribe(_handle);
		}

		template<typename... Args>
		void operator()(void* const _object, Args&&... _params) {
			if (m_ownerHandle != 0 && m_ownerHandle != (uintptr_t)_object) {
				LOG_ERROR("Event invoked by invalid owner");
				return;
			}

			Invoke(std::forward<Args>(_params)...);
		}

	private:

		std::shared_ptr<EventEntryHandle> Subscribe(EventFunctionT _func, void* const _object = nullptr)
		{
			std::shared_ptr<EventEntryHandle> handle = std::make_shared<EventEntryHandle>();
			handle->id = m_nextHandleId++;
			handle->isValid = true;

			m_eventFunctions.insert(std::make_pair(handle, _func));
			return handle;
		}

		void Unsubscribe(std::shared_ptr<EventEntryHandle> _handle)
		{
			auto it = std::find_if(m_eventFunctions.begin(), m_eventFunctions.end(), [&_handle](const auto& pair) {
				return pair.first == _handle;
			});

			if (it != m_eventFunctions.end())
			{
				m_eventFunctions.erase(it);
			}
		}

		template<typename... Args>
		void Invoke(Args&&... _params)
		{
			static_assert(std::is_invocable_v<EventFunctionT, Args...>, "Event invoked with invalid parameters");

			for (auto& [key, func] : m_eventFunctions)
			{
				func(std::forward<Args>(_params)...);
			}
		}

		std::map<std::shared_ptr<EventEntryHandle>, EventFunctionT> m_eventFunctions;
		std::vector<std::shared_ptr<EventEntryHandle>> m_handles;
		uintptr_t m_ownerHandle = 0;
		size_t m_nextHandleId = 1;
	};
}