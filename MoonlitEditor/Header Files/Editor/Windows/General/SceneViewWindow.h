#pragma once

#include "../EditorWindowBase.h"
#include "Camera.h"
#include "Editor/Controllers/SceneViewController.h"
#include "Editor/Windows/IDockManager.h"

class MoonlitEngine;
class QTimer;
class SceneViewInputHandler;
class RenderTarget;

class SceneViewWindow : public EditorWindowBase
{
public:
	SceneViewWindow(IDockManager* _dockManager);
	SceneViewWindow(QWidget* _parent = nullptr);

	bool event(QEvent* _event) override;

	//GETTERS
	HWND GetWindowHandle() const { return m_windowHandle; };
	HWND GetWidgetWindowHandle() const { return (HWND)effectiveWinId(); }
private:
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