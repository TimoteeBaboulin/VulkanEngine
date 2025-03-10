#include "InputManager.h"
#include <cwchar>
#include <winuser.h>
#include <iostream>

DWORD KeyDownSubscribeCallback(HWND _handle, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	wchar_t buffer[32];
	switch (uMsg)
	{
	case WM_KEYDOWN:
	{
		swprintf(buffer, 32, L"WM_SYSKEYDOWN: 0x%x\n", wParam);
		std::wcout << buffer << std::endl;
		break;
	}
	default:
		DefWindowProc(_handle, uMsg, wParam, lParam);
		break;
	}
	return 0;
}

void InputManager::Init(HWND _windowHandle)
{
	//SetWindowLongPtrA(_windowHandle, -4, (LONG_PTR)KeyDownSubscribeCallback);
}
