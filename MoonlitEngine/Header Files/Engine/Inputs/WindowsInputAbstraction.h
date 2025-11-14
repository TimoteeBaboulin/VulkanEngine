#pragma once

#include <Windows.h>
#include <map>

#include "PlatformInputAbstraction.h"

class QtEventReader;

/// <summary>
/// Windows-specific input abstraction singleton class
/// </summary>
class WindowsInputAbstraction : public PlatformInputAbstraction
{
private:
	static WindowsInputAbstraction* m_instance;
	static std::map<int, KEYBOARD_KEY> m_keyMap;
	static std::map<int, GAMEPAD_KEY> m_gamepadKeyMap;

	HWND m_windowHandle;
	RECT m_lockRect = { 0,0,0,0 };

	float m_mouseX = -1.f;
	float m_mouseY = -1.f;

	bool* m_axisActive;

	bool m_cursorLocked = false;

	void OnKeyboardInput(WPARAM _wParam, bool _keyDown, void* _winHandle);
	void OnMouseMove(POINT _point, void* _winHandle);
	void OnMouseClick(MOUSE_KEY _key, bool _keyDown, void* _winHandle);
	void OnGamepadInput(GAMEPAD_KEY _key, bool _keyDown, void* _winHandle);
	void OnGamepadAxis(GAMEPAD_KEY _key, float _x, float _y, void* _winHandle);


public:
	WindowsInputAbstraction(HWND _windowHandle);

	void HandleWindowsEvents(void* _handle, MSG _msg) override;

	void Init() override;
	void PollEvents() override;

	void LockCursor() override;
	void UnlockCursor() override;
};