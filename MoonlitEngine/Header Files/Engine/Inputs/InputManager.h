#pragma once

#include <vector>
#include <map>

#include <functional>

#include "Inputs.h"
#include "MoonlitExport.h"

//TODO: Allow handlers to subscribe to a specific window/event source
//TODO: Allow handlers to consume events so they don't propagate to other handlers

/// <summary>
/// Abstract class for handling input events
/// </summary>
class MOONLIT_API InputHandler
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
class QApplication;

using KeyboardInputCallback = std::function<void(KEYBOARD_KEY)>;
using GamepadInputCallback = std::function<void(GAMEPAD_KEY)>;
using MouseInputCallback = std::function<void(MOUSE_KEY)>;
using GamepadAxisInputCallback = std::function<void(GAMEPAD_KEY, float, float)>;
using AxisInputCallback = std::function<void(float, float)>;
using WindowEventCallback = std::function<void(WINDOW_EVENT, void*)>;	

/// <summary>
/// Platform agnostic singleton class used as an interface between input handlers<para/>
/// And platform specific input reading systems
/// </summary>
class MOONLIT_API InputManager
{
	// PlatformInputAbstraction is the base class for platform specific input handling
	// It also acts as the interface between the platform specific code and the manager
	friend class PlatformInputAbstraction;
public:
	// Static methods
	// Get instance does not have any safety checks
	// Because it is expected to be called often
	// Tests with other devs will ensure whether this should be changed or not

	static InputManager* GetInstance() { return m_instance; }

	/// <summary>
	/// This should always be called before accessing the instance.<para/>
	/// Calling GetInstance() without having called InitManager is undefined behaviour.
	/// </summary>
	/// <param name="_handle">The handle to the main window of the application</param>
	static void InitManager(void* _handle);

	// Subscribption methods
	// TODO: Add unsubscription methods

	void SubscribeKeyboardEvent(KEY_STATE _state, KeyboardInputCallback _callback);
	void SubscribeGamepadEvent(KEY_STATE _state, GamepadInputCallback _callback);
	void SubscribeGamepadAxisEvent(GamepadAxisInputCallback _callback);
	void SubscribeMouseEvent(KEY_STATE _state, MouseInputCallback _callback);
	void SubscribeWindowEvent(WindowEventCallback _callback);

	// Qt has its own event loop, and using the platform specific input handling
	// Can conflict with it, causing the application to react in unwanted ways

	void LinkQtApplication(QApplication* _qApp);

	// Update method

	void PollEvents();

	// Methods meant to interface easily with the platform abstraction

	void WindowResize(int _width, int _height);
	void WindowClose();

	void LockCursor();
	void UnlockCursor();

	// Handler management

	// This function doesn't do anything if the handler was already added
	void AddInputHandler(InputHandler* _handler);
	// This function doesn't do anything if the handler wasn't already added
	void RemoveInputHandler(InputHandler* _handler);

private:
	static InputManager* m_instance;

	PlatformInputAbstraction* m_platformInput;

	bool m_cursorLocked = false;

	float m_mouseX;
	float m_mouseY;

	// Input event subscriptions
	// This should not be used over the input handlers
	// But it is useful for classes that might need only a single event

	std::vector<KeyboardInputCallback>* m_keyboardInputCallbacks;
	std::vector<GamepadInputCallback>* m_gamepadInputCallbacks;
	std::vector<MouseInputCallback>* m_mouseInputCallbacks;
	std::vector<GamepadAxisInputCallback> m_gamepadAxisInputCallbacks;
	std::vector<AxisInputCallback> m_mouseAxisInputCallbacks;

	// Input handling methods
	// These are called by the platform abstraction

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
};