#include "Inputs/InputManager.h"
#include <cwchar>

#include <iostream>


#ifdef _WIN32

#include "Inputs/WindowsInputAbstraction.h"
#include <Windows.h>
#include <Xinput.h>

#endif

InputManager* InputManager::m_instance;

void InputManager::HandleMouseMovement(float _x, float _y)
{
	if (!m_inputHandlers.empty())
	{
		for (auto& handler : m_inputHandlers)
		{
			handler->HandleMouseMoveInput(_x, _y);
		}
	}
}

void InputManager::HandleMouseInput(MOUSE_KEY _key, bool _keyDown)
{
	if (!m_inputHandlers.empty())
	{
		for (auto& handler : m_inputHandlers)
		{
			handler->HandleMouseInput(_key, _keyDown);
		}
	}
}

void InputManager::HandleKeyboardInput(KEYBOARD_KEY _key, bool _keyDown) 
{
	if (!m_inputHandlers.empty())
	{
		for (auto& handler : m_inputHandlers)
		{
			handler->HandleKeyboardInput(_key, _keyDown);
		}
	}
}

void InputManager::HandleGamepadInput(GAMEPAD_KEY _key, bool _keyDown)
{
	if (!m_inputHandlers.empty())
	{
		for (auto& handler : m_inputHandlers)
		{
			handler->HandleGamepadInput(_key, _keyDown);
		}
	}
}

void InputManager::HandleGamepadAxis(GAMEPAD_KEY _key, float _value)
{
	if (!m_inputHandlers.empty())
	{
		for (auto& handler : m_inputHandlers)
		{
			handler->HandleGamepadAxis(_key, _value);
		}
	}

}

InputManager::InputManager(void* _handle)
{
#ifdef _WIN32
	m_platformInput = new WindowsInputAbstraction((HWND)_handle);
#endif // _WIN32


	m_platformInput->Init();
}

void InputManager::InitManager(void* _handle)
{
	if (m_instance == nullptr)
	{
		try
		{
			m_instance = new InputManager(_handle);
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
	m_platformInput->PollEvents();
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
	m_platformInput->LockCursor();
}

void InputManager::UnlockCursor()
{
	m_platformInput->UnlockCursor();
}