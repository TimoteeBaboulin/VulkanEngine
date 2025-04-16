#pragma once

#include <vector>
#include <map>

#include <functional>

#include "Inputs.h"

class InputHandler
{
public:
	virtual void HandleMouseMoveInput(int _deltaX, int _deltaY) = 0;
	virtual void HandleMouseInput(MOUSE_KEY _key, bool _keyDown) = 0;
	virtual void HandleKeyboardInput(KEYBOARD_KEY _key, bool _keyDown) = 0;
	virtual void HandleGamepadInput(GAMEPAD_KEY _key, bool _keyDown) = 0;
	virtual void HandleGamepadAxis(GAMEPAD_KEY _key, float _x, float _y) = 0;
};

enum struct WINDOW_EVENT : char
{
	WINDOW_RESIZE = 0,
	WINDOW_CLOSE = 1
};

class PlatformInputAbstraction;

typedef std::function<void(KEYBOARD_KEY)> KeyboardInputCallback;
typedef std::function<void(GAMEPAD_KEY)> GamepadInputCallback;
typedef std::function<void(MOUSE_KEY)> MouseInputCallback;
typedef std::function<void(GAMEPAD_KEY, float, float)> GamepadAxisInputCallback;
typedef std::function<void(float, float)> AxisInputCallback;
using WindowEventCallback = std::function<void(WINDOW_EVENT, void*)>;	

class InputManager
{
	friend class PlatformInputAbstraction;

private:
	static InputManager* m_instance;

	PlatformInputAbstraction* m_platformInput;

	bool m_cursorLocked = false;

	float m_mouseX;
	float m_mouseY;

	std::vector<KeyboardInputCallback>* m_keyboardInputCallbacks;
	std::vector<GamepadInputCallback>* m_gamepadInputCallbacks;
	std::vector<MouseInputCallback>* m_mouseInputCallbacks;
	std::vector<GamepadAxisInputCallback> m_gamepadAxisInputCallbacks;
	std::vector<AxisInputCallback> m_mouseAxisInputCallbacks;

	void HandleMouseMovement(float _x, float _y);
	void HandleMouseInput(MOUSE_KEY _key, bool _keyDown);
	void HandleKeyboardInput(KEYBOARD_KEY _key, bool _keyDown);
	void HandleGamepadInput(GAMEPAD_KEY _key, bool _keyDown);
	void HandleGamepadAxis(GAMEPAD_KEY _key, float _x, float _y);

	std::vector<InputHandler*> m_inputHandlers;
	std::map<KEYBOARD_KEY, KEY_STATE> m_keyboardKeyStates;
	std::map<MOUSE_KEY, KEY_STATE> m_mouseKeyStates;
	std::map<GAMEPAD_KEY, KEY_STATE> m_gamepadKeyStates;
	std::vector<WindowEventCallback> m_windowEventCallbacks;

	InputManager(void* _handle);

public:
	static InputManager* GetInstance() {return m_instance; }
	static void InitManager(void* _handle);

	void SubscribeKeyboardEvent(KEY_STATE _state, KeyboardInputCallback _callback);
	void SubscribeGamepadEvent(KEY_STATE _state, GamepadInputCallback _callback);
	void SubscribeGamepadAxisEvent(GamepadAxisInputCallback _callback);
	void SubscribeMouseEvent(KEY_STATE _state, MouseInputCallback _callback);
	void SubscribeWindowEvent(WindowEventCallback _callback);

	void PollEvents();
	void WindowResize(int _width, int _height);
	void WindowClose();

	void AddInputHandler(InputHandler* _handler);
	void RemoveInputHandler(InputHandler* _handler);

	void LockCursor();
	void UnlockCursor();
};