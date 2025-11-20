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

//InputInstance readers to dispatch to the right method

void InputManager::ReadInput(InputInstance _instance)
{
	switch (_instance.DeviceType)
	{
		case InputInstance::INPUT_DEVICE_TYPE::MOUSE:
		{
			ReadMouseInput(_instance);
			break;
		}
		case InputInstance::INPUT_DEVICE_TYPE::KEYBOARD:
		{
			ReadKeyboardInput(_instance);
			break;
		}
		case InputInstance::INPUT_DEVICE_TYPE::GAMEPAD:
		{
			ReadGamepadInput(_instance);
			break;
		}
		default:
			break;
	}
}

void InputManager::ReadMouseInput(InputInstance _instance)
{
	if (_instance.InputType == InputInstance::INPUT_STATE::AXIS)
	{
		float* rawInput = static_cast<float*>(_instance.RawInput);
		float _x = rawInput[0];
		float _y = rawInput[1];

		HandleMouseMovement(_x, _y, _instance.WindowHandle);
		return;
	}

	MOUSE_KEY* rawInput = static_cast<MOUSE_KEY*>(_instance.RawInput);
	bool keyDown = (_instance.InputType == InputInstance::INPUT_STATE::STARTED);
	HandleMouseInput(*rawInput, keyDown, _instance.WindowHandle);
}

void InputManager::ReadKeyboardInput(InputInstance _instance)
{
	KEYBOARD_KEY* rawInput = static_cast<KEYBOARD_KEY*>(_instance.RawInput);
	bool keyDown = (_instance.InputType == InputInstance::INPUT_STATE::STARTED);
	HandleKeyboardInput(*rawInput, keyDown, _instance.WindowHandle);
}

void InputManager::ReadGamepadInput(InputInstance _instance)
{
	if (_instance.InputType == InputInstance::INPUT_STATE::AXIS)
	{
		float* rawInput = static_cast<float*>(_instance.RawInput);
		GAMEPAD_KEY key = static_cast<GAMEPAD_KEY>(static_cast<int>(rawInput[0]));
		float _x = rawInput[1];
		float _y = rawInput[2];

		HandleGamepadAxis(key, _x, _y, _instance.WindowHandle);
		return;
	}

	GAMEPAD_KEY* rawInput = static_cast<GAMEPAD_KEY*>(_instance.RawInput);
	bool keyDown = (_instance.InputType == InputInstance::INPUT_STATE::STARTED);
	HandleGamepadInput(*rawInput, keyDown, _instance.WindowHandle);
}

//Input handling method calling the right handlers

void InputManager::HandleMouseMovement(float _x, float _y, void* _winHandle)
{
	std::vector<InputHandler*> inputHandlers = GetInputHandlersForWindow(_winHandle);

	if (!inputHandlers.empty())
	{
		for (auto& handler : inputHandlers)
		{
			handler->HandleMouseMoveInput(_x, _y);
		}
	}

	for (auto& callback : m_mouseAxisInputCallbacks)
	{
		callback(_x, _y);
	}
}

void InputManager::HandleMouseInput(MOUSE_KEY _key, bool _keyDown, void* _winHandle)
{
	std::vector<InputHandler*> inputHandlers = GetInputHandlersForWindow(_winHandle);

	if (!inputHandlers.empty())
	{
		for (auto& handler : inputHandlers)
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

void InputManager::HandleKeyboardInput(KEYBOARD_KEY _key, bool _keyDown, void* _winHandle)
{
	std::vector<InputHandler*> inputHandlers = GetInputHandlersForWindow(_winHandle);

	if (!inputHandlers.empty())
	{
		for (auto& handler : inputHandlers)
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

void InputManager::HandleGamepadInput(GAMEPAD_KEY _key, bool _keyDown, void* _winHandle)
{
	std::vector<InputHandler*> inputHandlers = GetInputHandlersForWindow(_winHandle);

	if (!inputHandlers.empty())
	{
		for (auto& handler : inputHandlers)
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

void InputManager::HandleGamepadAxis(GAMEPAD_KEY _key, float _x, float _y, void* _winHandle)
{
	std::vector<InputHandler*> inputHandlers = GetInputHandlersForWindow(_winHandle);

	if (!inputHandlers.empty())
	{
		for (auto& handler : inputHandlers)
		{
			handler->HandleGamepadAxis(_key, _x, _y);
		}
	}

	for (auto& callback : m_gamepadAxisInputCallbacks)
	{
		callback(_key, _x, _y);
	}
}

std::vector<InputHandler*> InputManager::GetInputHandlersForWindow(void* _winHandle)
{
	if (_winHandle == nullptr)
		return m_inputHandlers;

	std::vector<InputHandler*> handlersForWindow;
	//Use this to avoid re-allocating new arrays often in case we have many input handlers
	handlersForWindow.reserve(m_inputHandlers.Size());
	for (auto& handler : m_inputHandlers)
	{
		if (handler->GetWindowHandle() == nullptr || handler->GetWindowHandle() == _winHandle)
			handlersForWindow.push_back(handler);
	}

	return handlersForWindow;
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
	int* Size = new int[2];
	Size[0] = _width;
	Size[1] = _height;

	for (auto& callback : m_windowEventCallbacks)
	{
		callback(WINDOW_EVENT::WINDOW_RESIZE, Size);
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