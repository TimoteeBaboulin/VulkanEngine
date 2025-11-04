#include "Engine/Inputs/InputManager.h"
#include <cwchar>

#include <iostream>
#include <QtWidgets/qapplication.h>
#include "Debug/Logger.h"


#ifdef _WIN32
#include <Windows.h>
#undef MemoryBarrier
#include <Xinput.h>
#include "Engine/Inputs/WindowsInputAbstraction.h"
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

	for (auto& callback : m_mouseAxisInputCallbacks)
	{
		callback(_x, _y);
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

	auto it = m_mouseKeyStates.find(_key);

	if (it == m_mouseKeyStates.end())
	{
		m_mouseKeyStates.insert(std::make_pair(_key, _keyDown ? KEY_STATE::PRESSED : KEY_STATE::RELEASED));

		if (_keyDown)
		{
			std::vector<MouseInputCallback>& callbackList = m_mouseInputCallbacks[(int) KEY_STATE::PRESSED];

			for (auto& callback : callbackList)
			{
				callback(_key);
			}
		}
		else
		{
			std::vector<MouseInputCallback>& callbackList = m_mouseInputCallbacks[(int)KEY_STATE::RELEASED];

			for (auto& callback : callbackList)
			{
				callback(_key);
			}
		}

		return;
	}

	if (_keyDown)
	{
		if (it->second == KEY_STATE::RELEASED)
		{
			it->second = KEY_STATE::PRESSED;

			std::vector<MouseInputCallback>& callbackList = m_mouseInputCallbacks[(int)KEY_STATE::PRESSED];

			for (auto& callback : callbackList)
			{
				callback(_key);
			}
		}
	}
	else
	{
		if (it->second != KEY_STATE::RELEASED)
		{
			it->second = KEY_STATE::RELEASED;

			std::vector<MouseInputCallback>& callbackList = m_mouseInputCallbacks[(int)KEY_STATE::RELEASED];

			for (auto& callback : callbackList)
			{
				callback(_key);
			}
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

	auto it = m_keyboardKeyStates.find(_key);

	if (it == m_keyboardKeyStates.end())
	{
		m_keyboardKeyStates.insert(std::make_pair(_key, _keyDown ? KEY_STATE::PRESSED : KEY_STATE::RELEASED));

		if (_keyDown)
		{
			std::vector<KeyboardInputCallback>& callbackList = m_keyboardInputCallbacks[(int)KEY_STATE::PRESSED];

			for (auto& callback : callbackList)
			{
				callback(_key);
			}
		}
		else
		{
			std::vector<KeyboardInputCallback>& callbackList = m_keyboardInputCallbacks[(int)KEY_STATE::RELEASED];

			for (auto& callback : callbackList)
			{
				callback(_key);
			}
		}

		return;
	}

	if (_keyDown)
	{
		if (it->second == KEY_STATE::RELEASED)
		{
			it->second = KEY_STATE::PRESSED;

			std::vector<KeyboardInputCallback>& callbackList = m_keyboardInputCallbacks[(int)KEY_STATE::PRESSED];

			for (auto& callback : callbackList)
			{
				callback(_key);
			}
		}
	}
	else
	{
		if (it->second != KEY_STATE::RELEASED)
		{
			it->second = KEY_STATE::RELEASED;

			std::vector<KeyboardInputCallback>& callbackList = m_keyboardInputCallbacks[(int)KEY_STATE::RELEASED];

			for (auto& callback : callbackList)
			{
				callback(_key);
			}
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

	auto it = m_gamepadKeyStates.find(_key);

	if (it == m_gamepadKeyStates.end())
	{
		m_gamepadKeyStates.insert(std::make_pair(_key, _keyDown ? KEY_STATE::PRESSED : KEY_STATE::RELEASED));
		
		if (_keyDown)
		{
			std::vector<GamepadInputCallback>& callbackList = m_gamepadInputCallbacks[(int)KEY_STATE::PRESSED];

			for (auto& callback : callbackList)
			{
				callback(_key);
			}
		}
		else
		{
			std::vector<GamepadInputCallback>& callbackList = m_gamepadInputCallbacks[(int)KEY_STATE::RELEASED];

			for (auto& callback : callbackList)
			{
				callback(_key);
			}
		}

		return;
	}

	if (_keyDown)
	{
		if (it->second == KEY_STATE::RELEASED)
		{
			it->second = KEY_STATE::PRESSED;

			std::vector<GamepadInputCallback>& callbackList = m_gamepadInputCallbacks[(int)KEY_STATE::PRESSED];

			for (auto& callback : callbackList)
			{
				callback(_key);
			}
		}
	}
	else
	{
		if (it->second != KEY_STATE::RELEASED)
		{
			it->second = KEY_STATE::RELEASED;

			std::vector<GamepadInputCallback>& callbackList = m_gamepadInputCallbacks[(int)KEY_STATE::RELEASED];

			for (auto& callback : callbackList)
			{
				callback(_key);
			}
		}
	}
}

void InputManager::HandleGamepadAxis(GAMEPAD_KEY _key, float _x, float _y)
{
	if (!m_inputHandlers.empty())
	{
		for (auto& handler : m_inputHandlers)
		{
			handler->HandleGamepadAxis(_key, _x, _y);
		}
	}

	for (auto& callback : m_gamepadAxisInputCallbacks)
	{
		callback(_key, _x, _y);
	}
}

InputManager::InputManager(void* _handle)
{
#ifdef _WIN32
	m_platformInput = new WindowsInputAbstraction((HWND)_handle);
#endif // _WIN32

	m_mouseX = -1;
	m_mouseY = -1;
	m_platformInput->Init();

	m_keyboardInputCallbacks = new std::vector<KeyboardInputCallback>[3];
	m_gamepadInputCallbacks = new std::vector<GamepadInputCallback>[3];
	m_mouseInputCallbacks = new std::vector<MouseInputCallback>[3];
	m_gamepadInputCallbacks = new std::vector<GamepadInputCallback>[3];
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
			std::string errorMsg = "Failed to create Input Manager instance because of ";
			errorMsg += e.what();
			Logger::LogWarning(errorMsg.data());
			m_instance = nullptr;
		}
	}
}

void InputManager::SubscribeKeyboardEvent(KEY_STATE _state, KeyboardInputCallback _callback)
{
	m_keyboardInputCallbacks[(int)_state].push_back(_callback);
}

void InputManager::SubscribeGamepadEvent(KEY_STATE _state, GamepadInputCallback _callback)
{
	m_gamepadInputCallbacks[(int)_state].push_back(_callback);

}

void InputManager::SubscribeGamepadAxisEvent(GamepadAxisInputCallback _callback)
{
	m_gamepadAxisInputCallbacks.push_back(_callback);
}

void InputManager::SubscribeMouseEvent(KEY_STATE _state, MouseInputCallback _callback)
{
	m_mouseInputCallbacks[(int)_state].push_back(_callback);

}

void InputManager::SubscribeWindowEvent(WindowEventCallback _callback)
{
	m_windowEventCallbacks.push_back(_callback);
}

void InputManager::LinkQtApplication(QApplication* _qApp)
{
	m_platformInput->LinkQtApplication(_qApp);
}

void InputManager::PollEvents()
{
	m_platformInput->PollEvents();

	for (auto& key : m_keyboardKeyStates)
	{
		if (key.second == KEY_STATE::PRESSED)
		{
			key.second = KEY_STATE::HELD;

			for (auto& callback : m_keyboardInputCallbacks[(int)KEY_STATE::HELD])
			{
				callback(key.first);
			}
		}
	}

	for (auto& key : m_mouseKeyStates)
	{
		if (key.second == KEY_STATE::PRESSED)
		{
			key.second = KEY_STATE::HELD;

			for (auto& callback : m_mouseInputCallbacks[(int)KEY_STATE::HELD])
			{
				callback(key.first);
			}
		}
	}

	for (auto& key : m_gamepadKeyStates)
	{
		if (key.second == KEY_STATE::PRESSED)
		{
			key.second = KEY_STATE::HELD;

			for (auto& callback : m_gamepadInputCallbacks[(int)KEY_STATE::HELD])
			{
				callback(key.first);
			}
		}
	}
}

void InputManager::WindowResize(int _width, int _height)
{
	int* size = new int[2];
	size[0] = _width;
	size[1] = _height;

	for (auto& callback : m_windowEventCallbacks)
	{
		callback(WINDOW_EVENT::WINDOW_RESIZE, size);
	}
}

void InputManager::WindowClose()
{
	for (auto& callback : m_windowEventCallbacks)
	{
		callback(WINDOW_EVENT::WINDOW_CLOSE, nullptr);
	}
}

void InputManager::AddInputHandler(InputHandler* _handler)
{
	const auto& it = std::find(m_inputHandlers.begin(), m_inputHandlers.end(), _handler);
	if (it != m_inputHandlers.end())
	{
		return;
	}

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