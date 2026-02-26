#pragma once

#include "Inputs.h"

#include <Windows.h>

class QApplication;

namespace Moonlit::Inputs
{
	class QtEventReader;

	/// <summary>
	/// Class used as an abstraction layer for platform specific input handling
	/// </summary>
	class PlatformInputAbstraction
	{
	protected:
		// Interface with InputManager to avoid having to friend every platform implementation

		void SendInput(InputInstance _instance);

		void WindowResize(int _width, int _height);
		void WindowClose();

		QtEventReader* m_eventReader;

		virtual void HandleWindowsEvents(void* _handle, MSG _msg) = 0;

	public:
		virtual void Init() = 0;
		virtual void PollEvents() = 0;

		virtual void LockCursor() = 0;
		virtual void UnlockCursor() = 0;

		void LinkQtApplication(::QApplication* _qApp);
	};
}