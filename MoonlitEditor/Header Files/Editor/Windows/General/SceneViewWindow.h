#pragma once

#include "../EditorWindowBase.h"
#include "Camera.h"

class MoonlitEngine;
class QTimer;
class SceneViewInputHandler;
class RenderTarget;

class SceneViewWindow : public EditorWindowBase
{
public:
	SceneViewWindow(QWidget* _parent = nullptr);

	bool event(QEvent* _event) override;

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
	RenderTarget* m_renderTarget = nullptr;

	QTimer* m_resizeTimer = nullptr;

	bool resizing = false;

	void SetQtData();
	void StartResizing();
	void StopResizing();
};