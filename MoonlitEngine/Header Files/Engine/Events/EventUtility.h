#pragma once

#include "Event.h"

template<typename... paramTs>
class ScopedEventSubscriber
{
public:
	using EventT = Event<void, paramTs...>;
	ScopedEventSubscriber() = delete;
	ScopedEventSubscriber(EventT& _event, typename EventT::EventFunctionT _func)
		: m_event(_event)
	{
		m_handle = m_event += _func;
	}

	~ScopedEventSubscriber()
	{
		m_event -= m_handle;
	}

private:
	EventT& m_event;
	EventEntryHandle m_handle = 0;
};

// Used to avoid having to specify template parameters when creating a ScopedEventSubscriber
template<typename... paramTs>
ScopedEventSubscriber(Event<void, paramTs...>&, typename Event<void, paramTs...>::EventFunctionT)
	-> ScopedEventSubscriber<paramTs...>;