#include "Engine/Inputs/QtEventReader.h"
#include <iostream>
#include "Engine/Inputs/WindowsInputAbstraction.h" // adjust include path to your header

QtEventReader::QtEventReader(HWND targetHandle, PlatformInput_t* backend)
    : m_targetHandle(targetHandle), m_backend(backend)
{
}

QtEventReader::~QtEventReader()
{
}

bool QtEventReader::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result)
{
    return false;

    // We have to inverse the if otherwise the compiler can not choose the right override
    if (!(eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG"))
        return false;

    MSG* msg = reinterpret_cast<MSG*>(message);
    if (!msg || msg->hwnd != m_targetHandle)
        return false;

    if (m_backend) m_backend->HandleWindowsInputs(*msg);

    // Return false so Qt continues normal processing (important for resizing & non-client behavior).
    return false;
}