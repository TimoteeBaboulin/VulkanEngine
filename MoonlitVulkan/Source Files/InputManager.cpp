#include "InputManager.h"
#include <cwchar>
#include <winuser.h>
#include <iostream>

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
				std::wcout << L"Mouse move: " << pMouseStruct->pt.x << L", " << pMouseStruct->pt.y << std::endl;
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
		int deltaX = pMouseStruct->pt.x - m_mouseX;
		int deltaY = pMouseStruct->pt.y - m_mouseY;

		m_mouseX = pMouseStruct->pt.x;
		m_mouseY = pMouseStruct->pt.y;

		if (!m_inputHandlers.empty())
		{
			for (auto& handler : m_inputHandlers)
			{
				handler->HandleMouseMoveInput(deltaX, deltaY);
			}
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

InputManager::InputManager(HWND _windowHandle)
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
