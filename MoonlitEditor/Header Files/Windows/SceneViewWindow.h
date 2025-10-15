#pragma once

#include "Windows/EditorWindowBase.h"
#include "Camera.h"

class MoonlitEngine;
class QTimer;

class SceneViewWindow : public EditorWindowBase
{
public:
	SceneViewWindow(MoonlitEditor* _editor, MoonlitEngine* _engine, QWidget* _parent = nullptr);

	//GETTERS
	HWND GetWindowHandle() const { return m_windowHandle; };
private:
	QWidget* m_containerWidget = nullptr;
	HWND m_windowHandle = nullptr;
	MoonlitEngine* m_engine = nullptr;

	QTimer* m_updateTimer = nullptr;
	std::function<void()> m_updateCallback = nullptr;
	Camera m_camera;

	void SetQtData();
};