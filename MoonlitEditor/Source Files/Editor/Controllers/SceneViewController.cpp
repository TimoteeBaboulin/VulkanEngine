#include "Editor/Controllers/SceneViewController.h"
#include "Debug/Logger.h"

#include <string>

void SceneViewInputHandler::HandleMouseMoveInput(int _deltaX, int _deltaY)
{
	//DEBUG: Disable mouse movement handling for now
	return;


	const float mouseSensitivity = 0.1f;
	LOG_INFO(TEXTLOG("Mouse Move Input: DeltaX: " + std::to_string(_deltaX) + ", DeltaY: " + std::to_string(_deltaY)));
	m_camera.Rotate(m_camera.GetUpVector(), -_deltaX * mouseSensitivity);
	m_camera.Rotate(m_camera.GetRightVector(), _deltaY * mouseSensitivity);
}

void SceneViewInputHandler::HandleMouseInput(MOUSE_KEY _key, bool _keyDown)
{
	// No implementation needed for mouse button input in this handler
}

void SceneViewInputHandler::HandleKeyboardInput(KEYBOARD_KEY _key, bool _keyDown)
{
	return;
	//DEBUG: Disable keyboard movement handling for now

	const float moveSpeed = 0.5f;
	if (_keyDown)
	{
		switch (_key)
		{
		case KEYBOARD_KEY::Z:
			m_camera.Translate(m_camera.GetForwardVector() * moveSpeed);
			break;
		case KEYBOARD_KEY::S:
			m_camera.Translate(-m_camera.GetForwardVector() * moveSpeed);
			break;
		case KEYBOARD_KEY::Q:
			m_camera.Translate(-m_camera.GetRightVector() * moveSpeed);
			break;
		case KEYBOARD_KEY::D:
			m_camera.Translate(m_camera.GetRightVector() * moveSpeed);
			break;
		default:
			break;
		}
	}
}

void SceneViewInputHandler::HandleGamepadInput(GAMEPAD_KEY _key, bool _keyDown)
{
	// No implementation needed for gamepad button input in this handler
}

void SceneViewInputHandler::HandleGamepadAxis(GAMEPAD_KEY _key, float _x, float _y)
{
	return;
	//DEBUG: Disable gamepad movement handling for now
	const float moveSpeed = 0.5f;
	if (_key == GAMEPAD_KEY::AXIS_LEFT)
	{
		m_camera.Translate(m_camera.GetRightVector() * _x * moveSpeed);
		m_camera.Translate(m_camera.GetForwardVector() * -_y * moveSpeed);
	}
	else if (_key == GAMEPAD_KEY::AXIS_RIGHT)
	{
		m_camera.Rotate(m_camera.GetUpVector(), -_x * 2.0f);
		m_camera.Rotate(m_camera.GetRightVector(), -_y * 2.0f);
	}
}

SceneViewInputHandler::SceneViewInputHandler(Camera& camera) : m_camera(camera)
{
	InputManager::GetInstance()->AddInputHandler(this);
}

SceneViewInputHandler::~SceneViewInputHandler()
{
	InputManager::GetInstance()->RemoveInputHandler(this);
}
