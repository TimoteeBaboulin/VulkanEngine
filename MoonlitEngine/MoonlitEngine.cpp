#include "MoonlitEngine.h"

#include <QtCore/qtimer.h>

MoonlitEngine::MoonlitEngine() : m_engine()
{
}

MoonlitEngine::~MoonlitEngine()
{
}

void MoonlitEngine::SetWindowHandle(HWND _handle)
{
	m_windowHandle = _handle;
}

void MoonlitEngine::Init(const uint32_t _width, const uint32_t _height, std::string _windowTitle)
{
	ContextInfo context = {
		.name = _windowTitle.data(),
		.width = _width,
		.height = _height,
		.windowHandle = m_windowHandle
	};

	std::vector<const char*> extensions = { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME };
	m_engine.InitContext(context, extensions.data(), extensions.size());
	m_engine.InitVulkan();
}

void MoonlitEngine::ConnectToQTimer(QTimer* _timer)
{
	QObject::connect(_timer, &QTimer::timeout, [this]() {
		this->Update();
		});
}

void MoonlitEngine::Update()
{
	m_engine.Render();
}



MoonlitEngine::MoonlitEngine(const MoonlitEngine& _toCopy)
{
}
