#pragma once

#include <Windows.h>
#include <winevt.h>

static DWORD KeyDownSubscribeCallback(HWND _handle, UINT uMsg, WPARAM wParam, LPARAM lParam);

class InputManager
{
private:

public:
	void Init(HWND _windowHandle);
};