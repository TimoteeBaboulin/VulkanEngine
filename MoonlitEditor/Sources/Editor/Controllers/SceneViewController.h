#pragma once

#include "../../../../MoonlitEngine/Sources/Engine/Inputs/InputManager.h"
#include "../../../../MoonlitEngine/Sources/Engine/Renderer/Camera.h"

//TODO: Implement reading only from the scene view window inputs
//TODO: Once the input system has been refactored to support it

class SceneViewInputHandler : public Moonlit::Inputs::InputHandler
{
public:
	SceneViewInputHandler(Moonlit::Camera& _camera, void* _winHandle);
	~SceneViewInputHandler();

	void HandleMouseMoveInput(float _deltaX, float _deltaY) override;
	void HandleMouseInput(Moonlit::Inputs::MOUSE_KEY _key, bool _keyDown) override;
	void HandleKeyboardInput(Moonlit::Inputs::KEYBOARD_KEY _key, bool _keyDown) override;
	void HandleGamepadInput(Moonlit::Inputs::GAMEPAD_KEY _key, bool _keyDown) override;
	void HandleGamepadAxis(Moonlit::Inputs::GAMEPAD_KEY _key, float _x, float _y) override;

private:
	Moonlit::Camera& m_camera;

	bool m_mouseHeld = false;
};