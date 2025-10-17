#pragma once

#include "Windows/EditorWindowBase.h"
#include "Camera.h"

class MoonlitEngine;
class QTimer;
class SceneViewInputHandler;

class SceneViewWindow : public EditorWindowBase
{
public:
	SceneViewWindow(MoonlitEditor* _editor, MoonlitEngine* _engine, QWidget* _parent = nullptr);

	//GETTERS
	HWND GetWindowHandle() const { return m_windowHandle; };
	HWND GetWidgetWindowHandle() const { return (HWND)m_containerWidget->effectiveWinId(); }
private:
	QWidget* m_containerWidget = nullptr;
	HWND m_windowHandle = nullptr;
	MoonlitEngine* m_engine = nullptr;

	QTimer* m_updateTimer = nullptr;
	std::function<void()> m_updateCallback = nullptr;
	Camera m_camera;

	SceneViewInputHandler* m_inputHandler = nullptr;

	void SetQtData();
};