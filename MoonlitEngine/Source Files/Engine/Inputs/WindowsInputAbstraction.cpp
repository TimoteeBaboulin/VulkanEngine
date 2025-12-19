#include "Engine/Inputs/WindowsInputAbstraction.h"

#include <Windows.h>
#undef MemoryBarrier
#include <Xinput.h>
#include <windowsx.h>

#include <iostream>
#include <QtWidgets/qapplication.h>
#include "Engine/Inputs/QtEventReader.h"

WindowsInputAbstraction* WindowsInputAbstraction::m_instance = nullptr;

// Maps between windows' API enums and platform agnostic ones

std::map<int, KEYBOARD_KEY> WindowsInputAbstraction::m_keyMap =
{
	{VK_ESCAPE, KEYBOARD_KEY::ESCAPE},
	{VK_SPACE, KEYBOARD_KEY::SPACE},
	{VK_RETURN, KEYBOARD_KEY::ENTER},
	{VK_BACK, KEYBOARD_KEY::BACKSPACE},
	{VK_TAB, KEYBOARD_KEY::TAB},
	{VK_SHIFT, KEYBOARD_KEY::SHIFT},
	{VK_CONTROL, KEYBOARD_KEY::CTRL},
	{VK_MENU, KEYBOARD_KEY::ALT},
	{'A', KEYBOARD_KEY::A},
	{'B', KEYBOARD_KEY::B},
	{'C', KEYBOARD_KEY::C},
	{'D', KEYBOARD_KEY::D},
	{'E', KEYBOARD_KEY::E},
	{'F', KEYBOARD_KEY::F},
	{'G', KEYBOARD_KEY::G},
	{'H', KEYBOARD_KEY::H},
	{'I', KEYBOARD_KEY::I},
	{'J', KEYBOARD_KEY::J},
	{'K', KEYBOARD_KEY::K},
	{'L', KEYBOARD_KEY::L},
	{'M', KEYBOARD_KEY::M},
	{'N', KEYBOARD_KEY::N},
	{'O', KEYBOARD_KEY::O},
	{'P', KEYBOARD_KEY::P},
	{'Q', KEYBOARD_KEY::Q},
	{'R', KEYBOARD_KEY::R},
	{'S', KEYBOARD_KEY::S},
	{'T', KEYBOARD_KEY::T},
	{'U', KEYBOARD_KEY::U},
	{'V', KEYBOARD_KEY::V},
	{'W', KEYBOARD_KEY::W},
	{'X', KEYBOARD_KEY::X},
	{'Y', KEYBOARD_KEY::Y},
	{'Z', KEYBOARD_KEY::Z},
	{'0', KEYBOARD_KEY::NUM0},
	{'1', KEYBOARD_KEY::NUM1},
	{'2', KEYBOARD_KEY::NUM2},
	{'3', KEYBOARD_KEY::NUM3},
	{'4', KEYBOARD_KEY::NUM4},
	{'5', KEYBOARD_KEY::NUM5},
	{'6', KEYBOARD_KEY::NUM6},
	{'7', KEYBOARD_KEY::NUM7},
	{'8', KEYBOARD_KEY::NUM8},
	{'9', KEYBOARD_KEY::NUM9},
	{VK_NUMPAD0, KEYBOARD_KEY::ALPHANUM0},
	{VK_NUMPAD1, KEYBOARD_KEY::ALPHANUM1},
	{VK_NUMPAD2, KEYBOARD_KEY::ALPHANUM2},
	{VK_NUMPAD3, KEYBOARD_KEY::ALPHANUM3},
	{VK_NUMPAD4, KEYBOARD_KEY::ALPHANUM4},
	{VK_NUMPAD5, KEYBOARD_KEY::ALPHANUM5},
	{VK_NUMPAD6, KEYBOARD_KEY::ALPHANUM6},
	{VK_NUMPAD7, KEYBOARD_KEY::ALPHANUM7},
	{VK_NUMPAD8, KEYBOARD_KEY::ALPHANUM8},
	{VK_NUMPAD9, KEYBOARD_KEY::ALPHANUM9},
	{VK_DIVIDE, KEYBOARD_KEY::SLASH},
	{VK_MULTIPLY, KEYBOARD_KEY::ASTERISK},
	{VK_SUBTRACT, KEYBOARD_KEY::SUBTRACT},
	{VK_ADD, KEYBOARD_KEY::ADD},
	{VK_LWIN, KEYBOARD_KEY::LSYSTEM},
	{VK_RWIN, KEYBOARD_KEY::RSYSTEM},
	{VK_LEFT, KEYBOARD_KEY::ARROW_LEFT},
	{VK_RIGHT, KEYBOARD_KEY::ARROW_RIGHT},
	{VK_UP, KEYBOARD_KEY::ARROW_UP},
	{VK_DOWN, KEYBOARD_KEY::ARROW_DOWN}
};
std::map<int, GAMEPAD_KEY> WindowsInputAbstraction::m_gamepadKeyMap =
{
	{VK_PAD_DPAD_LEFT		, GAMEPAD_KEY::DPAD_LEFT	},
	{VK_PAD_DPAD_RIGHT		, GAMEPAD_KEY::DPAD_RIGHT	},
	{VK_PAD_DPAD_UP			, GAMEPAD_KEY::DPAD_UP		},
	{VK_PAD_DPAD_DOWN		, GAMEPAD_KEY::DPAD_DOWN	},
	{VK_PAD_START			, GAMEPAD_KEY::BUTTON_START },
	{VK_PAD_BACK			, GAMEPAD_KEY::BUTTON_SELECT},
	{VK_PAD_A				, GAMEPAD_KEY::BUTTON_A		},
	{VK_PAD_B				, GAMEPAD_KEY::BUTTON_B		},
	{VK_PAD_X				, GAMEPAD_KEY::BUTTON_X		},
	{VK_PAD_Y				, GAMEPAD_KEY::BUTTON_Y		},
	{VK_PAD_LSHOULDER		, GAMEPAD_KEY::BUTTON_LB	},
	{VK_PAD_RSHOULDER		, GAMEPAD_KEY::BUTTON_RB	},
	{VK_PAD_LTRIGGER		, GAMEPAD_KEY::BUTTON_LT	},
	{VK_PAD_RTRIGGER		, GAMEPAD_KEY::BUTTON_RT	},
	{VK_PAD_LTHUMB_DOWN		, GAMEPAD_KEY::AXIS_LEFT	},
	{VK_PAD_LTHUMB_UP		, GAMEPAD_KEY::AXIS_LEFT	},
	{VK_PAD_LTHUMB_LEFT		, GAMEPAD_KEY::AXIS_LEFT	},
	{VK_PAD_LTHUMB_RIGHT	, GAMEPAD_KEY::AXIS_LEFT	},
	{VK_PAD_LTHUMB_DOWNLEFT	, GAMEPAD_KEY::AXIS_LEFT	},
	{VK_PAD_LTHUMB_DOWNRIGHT, GAMEPAD_KEY::AXIS_LEFT	},
	{VK_PAD_LTHUMB_UPLEFT	, GAMEPAD_KEY::AXIS_LEFT	},
	{VK_PAD_LTHUMB_UPRIGHT	, GAMEPAD_KEY::AXIS_LEFT	},
	{VK_PAD_RTHUMB_DOWN		, GAMEPAD_KEY::AXIS_RIGHT	},
	{VK_PAD_RTHUMB_UP		, GAMEPAD_KEY::AXIS_RIGHT	},
	{VK_PAD_RTHUMB_LEFT		, GAMEPAD_KEY::AXIS_RIGHT	},
	{VK_PAD_RTHUMB_RIGHT	, GAMEPAD_KEY::AXIS_RIGHT	},
	{VK_PAD_RTHUMB_DOWNLEFT	, GAMEPAD_KEY::AXIS_RIGHT	},
	{VK_PAD_RTHUMB_DOWNRIGHT, GAMEPAD_KEY::AXIS_RIGHT	},
	{VK_PAD_RTHUMB_UPLEFT	, GAMEPAD_KEY::AXIS_RIGHT	},
	{VK_PAD_RTHUMB_UPRIGHT	, GAMEPAD_KEY::AXIS_RIGHT	},
};

void WindowsInputAbstraction::OnKeyboardInput(WPARAM _wParam, bool _keyDown, void* _winHandle)
{
	auto key = m_keyMap.find(_wParam);
	if (key != m_keyMap.end())
	{
		InputInstance instance;
		instance.DeviceType = InputInstance::INPUT_DEVICE_TYPE::KEYBOARD;
		instance.InputType = _keyDown ? InputInstance::INPUT_STATE::STARTED : InputInstance::INPUT_STATE::CANCELLED;
		instance.WindowHandle = _winHandle;
		instance.RawInput = (void*)&key->second;

		SendInput(instance);
	}
}

void WindowsInputAbstraction::OnMouseMove(POINT _point, void* _winHandle)
{
	int deltaX = 0;
	int deltaY = 0;

	ClientToScreen(m_windowHandle, &_point);

	if (m_cursorLocked)
	{
		int x = (m_lockRect.left + m_lockRect.right) / 2;
		int y = (m_lockRect.top + m_lockRect.bottom) / 2;

		deltaX = _point.x - x;
		deltaY = _point.y - y;

		SetCursorPos(x, y);
	}
	else
	{
		if (m_mouseX != -1.f && m_mouseY != -1.f)
		{
			deltaX = _point.x - m_mouseX;
			deltaY = _point.y - m_mouseY;
		}
	}

	m_mouseX = _point.x;
	m_mouseY = _point.y;

	InputInstance instance;
	instance.DeviceType = InputInstance::INPUT_DEVICE_TYPE::MOUSE;
	instance.InputType = InputInstance::INPUT_STATE::AXIS;
	instance.WindowHandle = _winHandle;
	float rawInput[2] = { (float)deltaX, (float)deltaY };
	instance.RawInput = (void*)rawInput;

	SendInput(instance);
}

void WindowsInputAbstraction::OnMouseClick(MOUSE_KEY _key, bool _keyDown, void* _winHandle)
{
	InputInstance instance;
	instance.DeviceType = InputInstance::INPUT_DEVICE_TYPE::MOUSE;
	instance.InputType = _keyDown ? InputInstance::INPUT_STATE::STARTED : InputInstance::INPUT_STATE::CANCELLED;
	instance.WindowHandle = _winHandle;
	instance.RawInput = (void*)&_key;
	SendInput(instance);
}

void WindowsInputAbstraction::OnGamepadInput(GAMEPAD_KEY _key, bool _keyDown, void* _winHandle)
{
	InputInstance instance;
	instance.DeviceType = InputInstance::INPUT_DEVICE_TYPE::GAMEPAD;
	instance.InputType = _keyDown ? InputInstance::INPUT_STATE::STARTED : InputInstance::INPUT_STATE::CANCELLED;
	instance.WindowHandle = _winHandle;
	instance.RawInput = (void*)&_key;
	SendInput(instance);
}

void WindowsInputAbstraction::OnGamepadAxis(GAMEPAD_KEY _key, float _x, float _y, void* _winHandle)
{
	InputInstance instance;
	instance.DeviceType = InputInstance::INPUT_DEVICE_TYPE::GAMEPAD;
	instance.InputType = InputInstance::INPUT_STATE::AXIS;
	instance.WindowHandle = _winHandle;
	float* rawInput = new float[3];
	rawInput[0] = static_cast<float>(static_cast<int>(_key));
	rawInput[1] = _x;
	rawInput[2] = _y;
	instance.RawInput = rawInput;
	SendInput(instance);
}

WindowsInputAbstraction::WindowsInputAbstraction(HWND _windowHandle) : m_windowHandle(_windowHandle)
{
	if (m_instance != nullptr)
	{
		throw new std::exception("There can only be one instance of WindowsInputAbstraction");
	}
	m_instance = this;
	m_axisActive = new bool[3] { false, false, false };

	m_eventReader = new QtEventReader(_windowHandle, this);
}

void WindowsInputAbstraction::HandleWindowsEvents(void* _handle, MSG _msg)
{
	wchar_t buffer[32];

	//Read the msg to debug it
	switch (_msg.message)
	{
	case WM_KEYDOWN:
	{
		OnKeyboardInput(_msg.wParam, true, _handle);
		break;
	}
	case WM_KEYUP:
	{
		OnKeyboardInput(_msg.wParam, false, _handle);
		break;
	}
	case WM_MOUSEMOVE:
	{
		OnMouseMove(POINT{ GET_X_LPARAM(_msg.lParam) , GET_Y_LPARAM(_msg.lParam) }, _handle);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		OnMouseClick(MOUSE_KEY::LEFT_CLICK, true, _handle);
		break;
	}
	case WM_LBUTTONUP:
	{
		OnMouseClick(MOUSE_KEY::LEFT_CLICK, false, _handle);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		OnMouseClick(MOUSE_KEY::RIGHT_CLICK, true, _handle);
		break;
	}
	case WM_RBUTTONUP:
	{
		OnMouseClick(MOUSE_KEY::RIGHT_CLICK, false, _handle);
		break;
	}
	case WM_CLOSE:
	{
		WindowClose();
		break;
	}
	default:
		break;
	}
}

void WindowsInputAbstraction::Init()
{
	//SetWindowLongPtrA(m_windowHandle, GWLP_WNDPROC, (LONG_PTR)WndProcCallback);
}

void WindowsInputAbstraction::PollEvents()
{
	GetClientRect(m_windowHandle, &m_lockRect);
	DWORD result(0);
	XINPUT_STATE state;
	for (DWORD gamepadIndex = 0; gamepadIndex < XUSER_MAX_COUNT; gamepadIndex++)
	{

		result = XInputGetState(gamepadIndex, &state);

		if (result != ERROR_SUCCESS)
		{
			continue;
		}

		DWORD reserved(0);
		XINPUT_KEYSTROKE keystroke;

		result = XInputGetKeystroke(gamepadIndex, reserved, &keystroke);

		if (result == ERROR_SUCCESS)
		{
			auto key = m_gamepadKeyMap.find(keystroke.VirtualKey);
			if (key != m_gamepadKeyMap.end())
			{
				bool isAxis = false;
				if (key->second == GAMEPAD_KEY::AXIS_LEFT)
				{
					m_axisActive[0] = state.Gamepad.sThumbLX != 0 || state.Gamepad.sThumbLY != 0;
					isAxis = true;
				}

				if (key->second == GAMEPAD_KEY::AXIS_RIGHT)
				{
					m_axisActive[1] = state.Gamepad.sThumbRX != 0 || state.Gamepad.sThumbRY != 0;
					isAxis = true;
				}

				// Ignore axis inputs here, they are handled separately
				if (!isAxis)
				{
					OnGamepadInput(key->second, (keystroke.Flags & XINPUT_KEYSTROKE_KEYDOWN) != 0, nullptr);
				}
			}
			else
			{
				//std::cout << "Gamepad input with code: " << keystroke.VirtualKey << std::endl;
			}
		}

		if (m_axisActive[0])
		{
			int rawX = state.Gamepad.sThumbLX;
			int rawY = state.Gamepad.sThumbLY;
			if (rawX * rawX + rawY * rawY <= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			{
				rawX = 0;
				rawY = 0;

				m_axisActive[0] = false;
			}

			OnGamepadAxis(GAMEPAD_KEY::AXIS_LEFT, state.Gamepad.sThumbLX / 32767.f, state.Gamepad.sThumbLY / 32767.f, nullptr);
		}

		if (m_axisActive[1])
		{
			int rawX = state.Gamepad.sThumbRX;
			int rawY = state.Gamepad.sThumbRY;
			if (rawX * rawX + rawY * rawY <= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			{
				rawX = 0;
				rawY = 0;

				m_axisActive[1] = false;
			}

			OnGamepadAxis(GAMEPAD_KEY::AXIS_RIGHT, state.Gamepad.sThumbRX / 32767.f, state.Gamepad.sThumbRY / 32767.f, nullptr);
		}
	}
}

void WindowsInputAbstraction::LockCursor()
{
	//TODO: This needs to be improved by allowing to lock to a specific renderTarget since these are the only windows the engine uses
	if (!GetClientRect(m_windowHandle, &m_lockRect))
	{
		//std::cout << "Couldn't lock the cursor because we couldn't get the screen rect" << std::endl;
		return;
	}
	ClipCursor(&m_lockRect);
	m_cursorLocked = true;
}

void WindowsInputAbstraction::UnlockCursor()
{
	ClipCursor(nullptr);
	m_cursorLocked = false;
}