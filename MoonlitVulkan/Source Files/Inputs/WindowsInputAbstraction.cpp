#include "Inputs/WindowsInputAbstraction.h"

#include <Windows.h>
#include <Xinput.h>

#include <iostream>

WindowsInputAbstraction* WindowsInputAbstraction::m_instance = nullptr;
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
	{VK_PAD_DPAD_LEFT, GAMEPAD_KEY::DPAD_LEFT },
	{VK_PAD_DPAD_RIGHT, GAMEPAD_KEY::DPAD_RIGHT },
	{VK_PAD_DPAD_UP, GAMEPAD_KEY::DPAD_UP },
	{VK_PAD_DPAD_DOWN, GAMEPAD_KEY::DPAD_DOWN },
	{VK_PAD_START, GAMEPAD_KEY::BUTTON_START },
	{VK_PAD_BACK, GAMEPAD_KEY::BUTTON_SELECT},
	{VK_PAD_A, GAMEPAD_KEY::BUTTON_A},
	{VK_PAD_B, GAMEPAD_KEY::BUTTON_B},
	{VK_PAD_X, GAMEPAD_KEY::BUTTON_X},
	{VK_PAD_Y, GAMEPAD_KEY::BUTTON_Y},
	{VK_PAD_LSHOULDER, GAMEPAD_KEY::BUTTON_LB},
	{VK_PAD_RSHOULDER, GAMEPAD_KEY::BUTTON_RB},
	{VK_PAD_LTRIGGER, GAMEPAD_KEY::BUTTON_LT},
	{VK_PAD_RTRIGGER, GAMEPAD_KEY::BUTTON_RT}
};


LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		WindowsInputAbstraction::m_instance->OnMouseEvent(wParam, lParam);
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
		WindowsInputAbstraction::m_instance->OnKeyboardInput(wParam, true);
		break;
	}
	case WM_KEYUP:
	{
		WindowsInputAbstraction::m_instance->OnKeyboardInput(wParam, false);
		break;
	}
	default:
		DefWindowProc(_handle, uMsg, wParam, lParam);
		break;
	}
	return 0;
}

void WindowsInputAbstraction::OnKeyboardInput(WPARAM _wParam, bool _keyDown)
{
	auto key = m_keyMap.find(_wParam);
	if (key != m_keyMap.end())
	{
		SendKeyboardInput(key->second, _keyDown);
	}
}

void WindowsInputAbstraction::OnMouseEvent(WPARAM _wParam, LPARAM _lParam)
{
	switch (_wParam)
	{
	case WM_MOUSEMOVE:
	case WM_NCMOUSEMOVE:
	{
		MSLLHOOKSTRUCT* pMouseStruct = (MSLLHOOKSTRUCT*)_lParam;
		int deltaX;
		int deltaY;

		if (m_cursorLocked)
		{
			int x = (m_lockRect.left + m_lockRect.right) / 2;
			int y = (m_lockRect.top + m_lockRect.bottom) / 2;

			deltaX = pMouseStruct->pt.x - x;
			deltaY = pMouseStruct->pt.y - y;

			SetCursorPos(x, y);
		}
		else
		{
			if (m_mouseX != -1.f && m_mouseY != -1.f)
			{
				deltaX = pMouseStruct->pt.x - m_mouseX;
				deltaY = pMouseStruct->pt.y - m_mouseY;
			}
		}

		m_mouseX = pMouseStruct->pt.x;
		m_mouseY = pMouseStruct->pt.y;

		SendMouseMovement(deltaX, deltaY);

		break;
	}
	default:
		break;
	}

}

WindowsInputAbstraction::WindowsInputAbstraction(HWND _windowHandle) : m_windowHandle(_windowHandle)
{
	if (m_instance != nullptr)
	{
		throw new std::exception("There can only be one instance of WindowsInputAbstraction");
	}
	m_instance = this;
}

void WindowsInputAbstraction::Init()
{
	SetWindowLongPtrA(m_windowHandle, -4, (LONG_PTR)KeyDownSubscribeCallback);
	SetWindowsHookA(WH_MOUSE, MouseProc);
}

void WindowsInputAbstraction::PollEvents()
{
	DWORD result(0);
	XINPUT_STATE state;
	for (DWORD gamepadIndex = 0; gamepadIndex < XUSER_MAX_COUNT; gamepadIndex++)
	{
		result = XInputGetState(gamepadIndex, &state);

		if (result != ERROR_SUCCESS)
		{
			continue;
		}

		SendGamepadAxis(GAMEPAD_KEY::AXIS_LEFT_X, state.Gamepad.sThumbLX / 32767.f);
		SendGamepadAxis(GAMEPAD_KEY::AXIS_LEFT_Y, state.Gamepad.sThumbLY / 32767.f);
		SendGamepadAxis(GAMEPAD_KEY::AXIS_RIGHT_X, state.Gamepad.sThumbRX / 32767.f);
		SendGamepadAxis(GAMEPAD_KEY::AXIS_RIGHT_Y, state.Gamepad.sThumbRY / 32767.f);

		DWORD reserved(0);
		XINPUT_KEYSTROKE keystroke;

		result = XInputGetKeystroke(gamepadIndex, reserved, &keystroke);

		if (result != ERROR_SUCCESS)
		{
			continue;
		}

		auto key = m_gamepadKeyMap.find(keystroke.VirtualKey);
		if (key != m_gamepadKeyMap.end())
		{
			SendGamepadInput(key->second, keystroke.Flags == XINPUT_KEYSTROKE_KEYDOWN);
		}
	}
}

void WindowsInputAbstraction::LockCursor()
{
	if (!GetWindowRect(m_windowHandle, &m_lockRect))
	{
		std::cout << "Couldn't lock the cursor because we couldn't get the screen rect" << std::endl;
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
