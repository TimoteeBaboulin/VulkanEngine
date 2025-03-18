#include "InputManager.h"
#include <cwchar>
#include <winuser.h>
#include <iostream>
#include <Xinput.h>

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		switch (wParam)
		{
			case WM_MOUSEMOVE:
			{
				//Called when mouse moves while holding the window
				MSLLHOOKSTRUCT* pMouseStruct = (MSLLHOOKSTRUCT*)lParam;
				//std::wcout << L"Mouse move: " << pMouseStruct->pt.x << L", " << pMouseStruct->pt.y << std::endl;
				break;
			}
			case WM_NCMOUSEMOVE:
			{
				//Called when mouse moves over the window
				InputManager::m_instance->HandleMouseInput(INPUTEVENTTYPE::MOUSEMOVE, lParam);
				break;
			}
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

DWORD KeyDownSubscribeCallback(HWND _handle, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	wchar_t buffer[32];
	switch (uMsg)
	{
	case WM_KEYDOWN:
	{
		InputManager::m_instance->HandleKeyboardInput(wParam, true);
		break;
	}
	case WM_KEYUP:
	{
		InputManager::m_instance->HandleKeyboardInput(wParam, false);
		break;
	}
	default:
		DefWindowProc(_handle, uMsg, wParam, lParam);
		break;
	}
	return 0;
}

InputManager* InputManager::m_instance;

void InputManager::HandleMouseInput(INPUTEVENTTYPE _type, LPARAM _param)
{
	switch (_type)
	{
	case INPUTEVENTTYPE::MOUSEMOVE:
	{
		MSLLHOOKSTRUCT* pMouseStruct = (MSLLHOOKSTRUCT*)_param;
		int deltaX;
		int deltaY;

		int x = (m_lockRect.left + m_lockRect.right) / 2;
		int y = (m_lockRect.top + m_lockRect.bottom) / 2;

		if (!m_cursorLocked)
		{
			deltaX = pMouseStruct->pt.x - m_mouseX;
			deltaY = pMouseStruct->pt.y - m_mouseY;

			m_mouseX = pMouseStruct->pt.x;
			m_mouseY = pMouseStruct->pt.y;
		}
		else
		{
			deltaX = pMouseStruct->pt.x - x;
			deltaY = pMouseStruct->pt.y - y;

			m_mouseX = x;
			m_mouseY = y;
		}
		
		

		if (!m_inputHandlers.empty())
		{
			for (auto& handler : m_inputHandlers)
			{
				handler->HandleMouseMoveInput(deltaX, deltaY);
			}
		}

		if (m_cursorLocked)
		{
			
			SetCursorPos(x, y);
		}

		break;
	}
	default:
		break;
	}
}

void InputManager::HandleKeyboardInput(int _key, bool _keyDown) 
{
	if (!m_inputHandlers.empty())
	{
		for (auto& handler : m_inputHandlers)
		{
			handler->HandleKeyboardInput(_key, _keyDown);
		}
	}
}

InputManager::InputManager(HWND _windowHandle) : m_winHandle(_windowHandle)
{
	SetWindowLongPtrA(_windowHandle, -4, (LONG_PTR)KeyDownSubscribeCallback);
	SetWindowsHookA(WH_MOUSE, MouseProc);
}

void InputManager::InitManager(HWND _windowHandle)
{
	if (m_instance == nullptr)
	{
		try
		{
			m_instance = new InputManager(_windowHandle);
		}
		catch (std::exception e)
		{
			std::cout << "Couldn't create Input Manager because of: " << e.what() << std::endl;
			m_instance = nullptr;
		}
	}
}

void InputManager::PollEvents()
{
	DWORD result;
	XINPUT_STATE state;
	for (DWORD gamepadIndex = 0; gamepadIndex < XUSER_MAX_COUNT; gamepadIndex++)
	{
		if (result != ERROR_SUCCESS)
		{
			continue;
		}

		DWORD reserved;
		XINPUT_KEYSTROKE keystroke;

		result = XInputGetKeystroke(gamepadIndex, reserved, &keystroke);

		if (result != ERROR_SUCCESS)
		{
			continue;
		}

		std::wcout << keystroke.VirtualKey << std::endl;

		//if (!m_inputHandlers.empty())
		//{
		//	for (auto& handler : m_inputHandlers)
		//	{
		//		handler->HandleGamepadInput(state);
		//	}
		//}
	}
	result = XInputGetState(0, &state);

	
}

void InputManager::AddInputHandler(InputHandler* _handler)
{
	m_inputHandlers.push_back(_handler);
}

void InputManager::RemoveInputHandler(InputHandler* _handler)
{
	const auto& it = std::find(m_inputHandlers.begin(), m_inputHandlers.end(), _handler);
	if (it == m_inputHandlers.end())
	{
		return;
	}

	m_inputHandlers.erase(it);
}

void InputManager::LockCursor()
{
	if (!GetWindowRect(m_winHandle, &m_lockRect))
	{
		std::cout << "Couldn't lock the cursor because we couldn't get the screen rect" << std::endl;
		return;
	}
	ClipCursor(&m_lockRect);
	m_cursorLocked = true;
}

void InputManager::UnlockCursor()
{
	ClipCursor(nullptr);
	m_cursorLocked = false;
}