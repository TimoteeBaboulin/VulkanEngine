#pragma once

#include "Inputs.h"

class QApplication;
class QtEventReader;

/// <summary>
/// Class used as an abstraction layer for platform specific input handling
/// </summary>
class PlatformInputAbstraction
{
protected:
	// Interface with InputManager to avoid having to friend every platform implementation

	void SendKeyboardInput(KEYBOARD_KEY _key, bool _keyDown);
	void SendMouseInput(MOUSE_KEY _key, bool _keyDown);
	void SendMouseMovement(float _x, float _y);
	void SendGamepadInput(GAMEPAD_KEY _key, bool _keyDown);
	void SendGamepadAxis(GAMEPAD_KEY _key, float _x, float _y);
	void WindowResize(int _width, int _height);
	void WindowClose();

	QtEventReader* m_eventReader;

public:
	virtual void Init() = 0;
	virtual void PollEvents() = 0;

	virtual void LockCursor() = 0;
	virtual void UnlockCursor() = 0;

	void LinkQtApplication(QApplication* _qApp);
};