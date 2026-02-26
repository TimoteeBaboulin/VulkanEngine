#pragma once

#include <functional>
#include <map>

#include "../../../Sources/Debug/Logger.h"
#include "../../../Sources/MoonlitExport.h"

namespace Moonlit::Events
{

	using EventEntryHandle = size_t;

	template<typename ReturnT, typename... paramTs>
	class Event
	{
	public:
		using EventFunctionT = std::function<ReturnT(paramTs...)>;

		Event()
		{
			m_ownerHandle = 0;
			m_nextHandle = 1;
		}
		Event(void* const _owner)
		{
			m_ownerHandle = reinterpret_cast<uintptr_t>(_owner);
			m_nextHandle = 1;
		}

		EventEntryHandle operator+=(EventFunctionT _func)
		{
			return Subscribe(_func);
		}
		void operator-=(EventEntryHandle _handle)
		{
			Unsubscribe(_handle);
		}

		EventEntryHandle Subscribe(EventFunctionT _func, void* const _object = nullptr)
		{
			EventEntryHandle handle = m_nextHandle++;
			m_eventFunctions.insert(std::make_pair(handle, _func));
			return handle;
		}
		void Unsubscribe(EventEntryHandle _handle)
		{
			auto it = m_eventFunctions.find(_handle);

			if (it != m_eventFunctions.end())
			{
				m_eventFunctions.erase(it);
			}
		}

		template<typename... Args>
		void Invoke(void* const _object, Args&&... _params);

	private:
		std::map<EventEntryHandle, EventFunctionT> m_eventFunctions;
		uintptr_t m_ownerHandle = 0;
		EventEntryHandle m_nextHandle = 1;
	};

	template<typename ReturnT, typename ...paramTs>
	template<typename ...Args>
	inline void Event<ReturnT, paramTs...>::Invoke(void* const _object, Args && ..._params)
	{
		static_assert(std::is_invocable_v<EventFunctionT, Args...>, "Event invoked with invalid parameters");

		uintptr_t handle = reinterpret_cast<uintptr_t>(_object);
		if (m_ownerHandle != 0 && handle != m_ownerHandle)
		{
			LOG_ERROR("Event invoked by invalid object");
			return;
		}
		for (auto& [key, func] : m_eventFunctions)
		{
			func(std::forward<Args>(_params)...);
		}
	}
}