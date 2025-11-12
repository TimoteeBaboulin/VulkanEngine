#pragma once

#include <vector>
#include <functional>

#include "Engine/Events/Event.h"
#include "Engine/GameObject.h"

class EditorWindowBase;
class EditorMainWindow;
class MoonlitEngine;

class QApplication;
class QTimer;

namespace ads
{
	class CDockManager;
}

class MoonlitEditor
{
public:
	static MoonlitEditor* Editor;

	static Event<void, GameObject*>& OnSelectionChanged()
	{
		static Event<void, GameObject*> instance;
	    return instance;
	}

	MoonlitEditor();

	MoonlitEngine& GetEngine() { return *m_engine; }
	QApplication& GetApplication() { return *m_app; }
private:
	EditorMainWindow* m_mainWindow = nullptr;
	MoonlitEngine* m_engine = nullptr;

	QApplication* m_app = nullptr;
	QTimer* m_updateTimer = nullptr;
	std::function<void()> m_updateCallback;

	std::vector<EditorWindowBase*> m_editorWindows;

	class DefaultDockManager* m_dockManager;
};