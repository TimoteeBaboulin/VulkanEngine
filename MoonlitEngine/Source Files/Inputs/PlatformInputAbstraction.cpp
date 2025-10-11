#include "Inputs/PlatformInputAbstraction.h"
#include "Inputs/InputManager.h"

void PlatformInputAbstraction::SendKeyboardInput(KEYBOARD_KEY _key, bool _keyDown)
{
	InputManager::m_instance->HandleKeyboardInput(_key, _keyDown);
}

void PlatformInputAbstraction::SendMouseInput(MOUSE_KEY _key, bool _keyDown)
{
	InputManager::m_instance->HandleMouseInput(_key, _keyDown);
}

void PlatformInputAbstraction::SendMouseMovement(float _x, float _y)
{
	InputManager::m_instance->HandleMouseMovement(_x, _y);
}

void PlatformInputAbstraction::SendGamepadInput(GAMEPAD_KEY _key, bool _keyDown)
{
	InputManager::m_instance->HandleGamepadInput(_key, _keyDown);
}

void PlatformInputAbstraction::SendGamepadAxis(GAMEPAD_KEY _key, float _x, float _y)
{
	InputManager::m_instance->HandleGamepadAxis(_key, _x, _y);
}

void PlatformInputAbstraction::WindowResize(int _width, int _height)
{
	InputManager::m_instance->WindowResize(_width, _height);
}

void PlatformInputAbstraction::WindowClose()
{
	InputManager::m_instance->WindowClose();
}
