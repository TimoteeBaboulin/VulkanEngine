#pragma once

#include "PlatformInputAbstraction.h"

#include "Windows.h"
#include "map"

class WindowsInputAbstraction : public PlatformInputAbstraction
{
	friend LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
	friend DWORD KeyDownSubscribeCallback(HWND _handle, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	static WindowsInputAbstraction* m_instance;
	static std::map<int, KEYBOARD_KEY> m_keyMap;
	static std::map<int, GAMEPAD_KEY> m_gamepadKeyMap;

	HWND m_windowHandle;
	RECT m_lockRect = { 0,0,0,0 };

	float m_mouseX = -1.f;
	float m_mouseY = -1.f;

	bool m_cursorLocked = false;

	void OnKeyboardInput(WPARAM _wParam, bool _keyDown);
	void OnMouseEvent(WPARAM _wParam, LPARAM _lParam);

public:
	WindowsInputAbstraction(HWND _windowHandle);

	void Init() override;
	void PollEvents() override;

	void LockCursor() override;
	void UnlockCursor() override;
};