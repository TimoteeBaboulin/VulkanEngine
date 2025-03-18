#pragma once

#include <Windows.h>
#include <winevt.h>

#include <vector>
#include <map>

static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
static DWORD KeyDownSubscribeCallback(HWND _handle, UINT uMsg, WPARAM wParam, LPARAM lParam);

enum class INPUTEVENTTYPE
{
	KEYDOWN,
	KEYUP,
	MOUSEMOVE,
	MOUSEWHEEL,
	MOUSEBUTTONDOWN,
	MOUSEBUTTONUP
};

class InputHandler
{
public:
	virtual void HandleMouseMoveInput(int _deltaX, int _deltaY) = 0;
	virtual void HandleKeyboardInput(int _key, bool _keyDown) = 0;
	virtual void HandleGamepadInput(WORD _key, bool _keyDown) = 0;
};

class InputManager
{
	friend LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
	friend DWORD KeyDownSubscribeCallback(HWND _handle, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	static InputManager* m_instance;

	bool m_cursorLocked = false;

	RECT m_lockRect = { 0,0,0,0 };
	const HWND m_winHandle;

	float m_mouseX;
	float m_mouseY;

	void HandleMouseInput(INPUTEVENTTYPE _type, LPARAM _param);
	void HandleKeyboardInput(int _key, bool _keyDown);

	std::vector<InputHandler*> m_inputHandlers;

	InputManager(HWND _windowHandle);

public:
	static InputManager* GetInstance() {return m_instance; }
	static void InitManager(HWND _windowHandle);

	void PollEvents();

	void AddInputHandler(InputHandler* _handler);
	void RemoveInputHandler(InputHandler* _handler);

	void LockCursor();
	void UnlockCursor();
};