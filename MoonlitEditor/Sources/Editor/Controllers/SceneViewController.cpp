#include "SceneViewController.h"
#include "../../../../MoonlitEngine/Sources/Debug/Logger.h"

#include <string>

void SceneViewInputHandler::HandleMouseMoveInput(float _deltaX, float _deltaY)
{
	//DEBUG: Disable mouse movement handling for now

	if (!m_mouseHeld)
		return;

	const float mouseSensitivity = 0.05f;

	m_camera.Rotate(m_camera.GetUpVector(), -_deltaX * mouseSensitivity);
	m_camera.Rotate(m_camera.GetRightVector(), _deltaY * mouseSensitivity);
}

void SceneViewInputHandler::HandleMouseInput(Moonlit::Inputs::MOUSE_KEY _key, bool _keyDown)
{
	if (_key == Moonlit::Inputs::MOUSE_KEY::LEFT_CLICK)
	{
		m_mouseHeld = _keyDown;
	}
}

void SceneViewInputHandler::HandleKeyboardInput(Moonlit::Inputs::KEYBOARD_KEY _key, bool _keyDown)
{
	return;
	//DEBUG: Disable keyboard movement handling for now

	const float moveSpeed = 0.5f;
	if (_keyDown)
	{
		switch (_key)
		{
		case Moonlit::Inputs::KEYBOARD_KEY::Z:
			m_camera.Translate(m_camera.GetForwardVector() * moveSpeed);
			break;
		case Moonlit::Inputs::KEYBOARD_KEY::S:
			m_camera.Translate(-m_camera.GetForwardVector() * moveSpeed);
			break;
		case Moonlit::Inputs::KEYBOARD_KEY::Q:
			m_camera.Translate(-m_camera.GetRightVector() * moveSpeed);
			break;
		case Moonlit::Inputs::KEYBOARD_KEY::D:
			m_camera.Translate(m_camera.GetRightVector() * moveSpeed);
			break;
		default:
			break;
		}
	}
}

void SceneViewInputHandler::HandleGamepadInput(Moonlit::Inputs::GAMEPAD_KEY _key, bool _keyDown)
{
	// No implementation needed for gamepad button input in this handler
}

void SceneViewInputHandler::HandleGamepadAxis(Moonlit::Inputs::GAMEPAD_KEY _key, float _x, float _y)
{
	return;
	//DEBUG: Disable gamepad movement handling for now
	const float moveSpeed = 0.5f;
	if (_key == Moonlit::Inputs::GAMEPAD_KEY::AXIS_LEFT)
	{
		m_camera.Translate(m_camera.GetRightVector() * _x * moveSpeed);
		m_camera.Translate(m_camera.GetForwardVector() * -_y * moveSpeed);
	}
	else if (_key == Moonlit::Inputs::GAMEPAD_KEY::AXIS_RIGHT)
	{
		m_camera.Rotate(m_camera.GetUpVector(), -_x * 2.0f);
		m_camera.Rotate(m_camera.GetRightVector(), -_y * 2.0f);
	}
}

SceneViewInputHandler::SceneViewInputHandler(Moonlit::Camera& _camera, void* _winHandle)
	: m_camera(_camera)
{
	m_windowHandle = _winHandle;
	Moonlit::Inputs::InputManager::GetInstance()->AddInputHandler(this);
}

SceneViewInputHandler::~SceneViewInputHandler()
{
	Moonlit::Inputs::InputManager::GetInstance()->RemoveInputHandler(this);
}
