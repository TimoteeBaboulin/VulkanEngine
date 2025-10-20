#include "SceneViewInputHandler.h"
#include <iostream>

void SceneViewInputHandler::HandleMouseMoveInput(int _deltaX, int _deltaY)
{
	if (!m_mouseHeld) return;
	m_camera->Rotate(glm::vec3(0.0f, -1.0f, 0.0f), _deltaX * 0.01f);
	m_camera->Rotate(glm::vec3(-1.0f, 0.0f, 0.0f), _deltaY * 0.01f);
	std::cout << "Camera is rotating" << std::endl;
}

void SceneViewInputHandler::HandleMouseInput(MOUSE_KEY _key, bool _keyDown)
{
	switch (_key)
	{
	case MOUSE_KEY::LEFT_CLICK:
		m_mouseHeld = _keyDown;
		std::cout << "Mouse is ";
		if (!m_mouseHeld) std::cout << "not ";
		std::cout << "held down" << std::endl;
		break;
	default:
		break;
	}
}

void SceneViewInputHandler::HandleKeyboardInput(KEYBOARD_KEY _key, bool _keyDown)
{
	switch (_key)
	{
	case KEYBOARD_KEY::ARROW_LEFT:
		m_camera->Translate(-m_camera->GetRightVector());
		break;
	case KEYBOARD_KEY::ESCAPE:
		InputManager::GetInstance()->UnlockCursor();
		break;
	case KEYBOARD_KEY::SPACE:
		InputManager::GetInstance()->LockCursor();
		break;
	}
}

void SceneViewInputHandler::HandleGamepadInput(GAMEPAD_KEY _key, bool _keyDown)
{
	switch (_key)
	{
	case GAMEPAD_KEY::DPAD_LEFT:
		m_camera->Translate(-m_camera->GetRightVector());
		break;
	case GAMEPAD_KEY::DPAD_RIGHT:
		m_camera->Translate(m_camera->GetRightVector());
		break;
	default:
		break;
	}
}

void SceneViewInputHandler::HandleGamepadAxis(GAMEPAD_KEY _key, float _x, float _y)
{
	switch (_key)
	{
	case AXIS_RIGHT:
		m_camera->Rotate(glm::vec3(0.0f, -1.0f, 0.0f), _x * 0.1f);
		m_camera->Rotate(glm::vec3(1.0f, 0.0f, 0.0f), _y * 0.1f);
		break;
	case AXIS_LEFT:
	{
		glm::vec3 translation = m_camera->GetForwardVector() * _y + m_camera->GetRightVector() * _x;
		m_camera->Translate(translation);
		break;
	}
	default:
		break;
	}
}
