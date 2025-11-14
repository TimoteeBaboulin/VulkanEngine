#include "Engine/Inputs/PlatformInputAbstraction.h"
#include <QtWidgets/qapplication.h>
#include "Engine/Inputs/InputManager.h"
#include "Engine/Inputs/QtEventReader.h"

void PlatformInputAbstraction::SendInput(InputInstance _instance)
{
	InputManager::m_instance->ReadInput(_instance);
}

void PlatformInputAbstraction::WindowResize(int _width, int _height)
{
	InputManager::m_instance->WindowResize(_width, _height);
}

void PlatformInputAbstraction::WindowClose()
{
	InputManager::m_instance->WindowClose();
}

void PlatformInputAbstraction::LinkQtApplication(QApplication* _qApp)
{
	_qApp->installNativeEventFilter(m_eventReader);
}
