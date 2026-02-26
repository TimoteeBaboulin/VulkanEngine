#pragma once
#include <QtCore/QAbstractNativeEventFilter>
#include <QtCore/QByteArray>
#include <windows.h>

#ifdef _WIN32
#include "WindowsInputAbstraction.h"
#else
#include "PlatformInputAbstraction.h"
#endif

namespace Moonlit::Inputs
{

#ifdef _WIN32
    using PlatformInput_t = WindowsInputAbstraction;
#else
    using PlatformInput_t = PlatformInputAbstraction;
#endif

    /// <summary>
    /// Qt event reader class used to avoid event reading conflicts between platform specific code and Qt
    /// </summary>
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
}