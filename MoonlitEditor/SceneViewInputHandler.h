#pragma once

#include "Engine/Inputs/InputManager.h"
#include "Camera.h"

class SceneViewInputHandler : public InputHandler
{
private:
	Camera* m_camera;
	bool m_mouseHeld;

public:
	SceneViewInputHandler(Camera* _camera) : m_camera(_camera), m_mouseHeld(false) {}

	void HandleMouseMoveInput(int _deltaX, int _deltaY) override;
	void HandleMouseInput(MOUSE_KEY _key, bool _keyDown) override;
	void HandleKeyboardInput(KEYBOARD_KEY _key, bool _keyDown) override;
	void HandleGamepadInput(GAMEPAD_KEY _key, bool _keyDown) override;
	void HandleGamepadAxis(GAMEPAD_KEY _key, float _x, float _y) override;
};