#pragma once

#include <vector>
#include <map>

#include "Inputs.h"

class InputHandler
{
public:
	virtual void HandleMouseMoveInput(int _deltaX, int _deltaY) = 0;
	virtual void HandleMouseInput(MOUSE_KEY _key, bool _keyDown) = 0;
	virtual void HandleKeyboardInput(KEYBOARD_KEY _key, bool _keyDown) = 0;
	virtual void HandleGamepadInput(GAMEPAD_KEY _key, bool _keyDown) = 0;
	virtual void HandleGamepadAxis(GAMEPAD_KEY _key, float _value) = 0;
};

class PlatformInputAbstraction;

class InputManager
{
	friend class PlatformInputAbstraction;

private:
	static InputManager* m_instance;

	PlatformInputAbstraction* m_platformInput;

	bool m_cursorLocked = false;

	float m_mouseX;
	float m_mouseY;

	void HandleMouseMovement(float _x, float _y);
	void HandleMouseInput(MOUSE_KEY _key, bool _keyDown);
	void HandleKeyboardInput(KEYBOARD_KEY _key, bool _keyDown);
	void HandleGamepadInput(GAMEPAD_KEY _key, bool _keyDown);
	void HandleGamepadAxis(GAMEPAD_KEY _key, float _value);

	std::vector<InputHandler*> m_inputHandlers;

	InputManager(void* _handle);

public:
	static InputManager* GetInstance() {return m_instance; }
	static void InitManager(void* _handle);

	void PollEvents();

	void AddInputHandler(InputHandler* _handler);
	void RemoveInputHandler(InputHandler* _handler);

	void LockCursor();
	void UnlockCursor();
};