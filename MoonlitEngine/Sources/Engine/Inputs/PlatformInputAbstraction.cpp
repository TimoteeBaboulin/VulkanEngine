#include "PlatformInputAbstraction.h"
#include "InputManager.h"
#include "QtEventReader.h"

#include <QtWidgets/qapplication.h>

void Moonlit::Inputs::PlatformInputAbstraction::SendInput(InputInstance _instance)
{
	InputManager::m_instance->ReadInput(_instance);
}

void Moonlit::Inputs::PlatformInputAbstraction::WindowResize(int _width, int _height)
{
	InputManager::m_instance->WindowResize(_width, _height);
}

void Moonlit::Inputs::PlatformInputAbstraction::WindowClose()
{
	InputManager::m_instance->WindowClose();
}

void Moonlit::Inputs::PlatformInputAbstraction::LinkQtApplication(::QApplication* _qApp)
{
	_qApp->installNativeEventFilter(m_eventReader);
}
