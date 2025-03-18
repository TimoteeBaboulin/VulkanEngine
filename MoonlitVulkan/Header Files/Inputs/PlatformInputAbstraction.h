#pragma once

#include "Inputs.h"

class PlatformInputAbstraction
{
protected:
	void SendKeyboardInput(KEYBOARD_KEY _key, bool _keyDown);
	void SendMouseInput(MOUSE_KEY _key, bool _keyDown);
	void SendMouseMovement(float _x, float _y);
	void SendGamepadInput(GAMEPAD_KEY _key, bool _keyDown);
	void SendGamepadAxis(GAMEPAD_KEY _key, float _value);

public:
	virtual void Init() = 0;
	virtual void PollEvents() = 0;

	virtual void LockCursor() = 0;
	virtual void UnlockCursor() = 0;
};