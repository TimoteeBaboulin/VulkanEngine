#pragma once

#include "Engine/Inputs/InputManager.h"
#include "Camera.h"

//TODO: Implement reading only from the scene view window inputs
//TODO: Once the input system has been refactored to support it

class SceneViewInputHandler : public InputHandler
{
public:
	SceneViewInputHandler(Camera& camera);
	~SceneViewInputHandler();

	void HandleMouseMoveInput(int _deltaX, int _deltaY) override;
	void HandleMouseInput(MOUSE_KEY _key, bool _keyDown) override;
	void HandleKeyboardInput(KEYBOARD_KEY _key, bool _keyDown) override;
	void HandleGamepadInput(GAMEPAD_KEY _key, bool _keyDown) override;
	void HandleGamepadAxis(GAMEPAD_KEY _key, float _x, float _y) override;

private:
	Camera& m_camera;
};