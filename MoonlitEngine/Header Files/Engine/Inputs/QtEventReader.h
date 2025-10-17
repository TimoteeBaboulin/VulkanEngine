#pragma once
#include <QtCore/QAbstractNativeEventFilter>
#include <QtCore/QByteArray>
#include <windows.h>

#ifdef _WIN32
#include "WindowsInputAbstraction.h"
using PlatformInput_t = WindowsInputAbstraction;
#else
#include "PlatformInputAbstraction.h"
using PlatformInput_t = PlatformInputAbstraction;
#endif

class WindowsInputAbstraction; // forward; or use InputManager pointer / callback

class QtEventReader : public QAbstractNativeEventFilter
{
public:
    QtEventReader(HWND targetHandle, PlatformInput_t* backend);
    ~QtEventReader() override;
    bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) override;

private:
    HWND m_targetHandle;
    PlatformInput_t* m_backend;
};