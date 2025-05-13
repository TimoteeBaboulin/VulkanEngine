//#pragma once
//#include <QtCore/qobject.h>
//
//#include <windows.h>
//#include "Renderer/VulkanEngine.h"
//
//class GameObject;
//
//class MoonlitEngine : public QObject
//{
//public:
//	MoonlitEngine();
//	~MoonlitEngine();
//
//	void SetWindowHandle(HWND _handle);
//	void Init(const uint32_t _width, const uint32_t _height, std::string _windowTitle);
//	void ConnectToQTimer(QTimer* _timer);
//
//	void Update();
//
//	MoonlitEngine(const MoonlitEngine& _toCopy);
//
//private:
//	HWND m_windowHandle;
//	VulkanEngine m_engine;
//
//	std::vector<GameObject> m_gameObjects;
//};