#pragma once

#include "../EditorWindowBase.h"
#include "../../../../../MoonlitEngine/Sources/Engine/Renderer/Camera.h"
#include "../../Controllers/SceneViewController.h"
#include "../IDockManager.h"

namespace Moonlit
{
	class MoonlitEngine;
	namespace Renderer
	{
		class RenderTarget;
	}
}

class QTimer;
class SceneViewInputHandler;

class SceneView : public EditorWindowBase
{
public:
	SceneView(IDockManager* _dockManager);
	SceneView(QWidget* _parent = nullptr);

	bool event(QEvent* _event) override;

	//GETTERS
	HWND GetWindowHandle() const { return m_windowHandle; };
	HWND GetWidgetWindowHandle() const { return (HWND)effectiveWinId(); }
private:
	HWND m_windowHandle = nullptr;
	Moonlit::MoonlitEngine* m_engine = nullptr;

	Moonlit::Camera m_camera;

	SceneViewInputHandler* m_inputHandler = nullptr;
	Moonlit::Renderer::RenderTarget* m_renderTarget = nullptr;

	QTimer* m_resizeTimer = nullptr;

	bool resizing = false;

	void SetQtData();
	void StartResizing();
	void StopResizing();
};