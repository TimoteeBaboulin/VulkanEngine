#pragma once

#include <vector>
#include <functional>

#include "Engine/Events/Event.h"
#include "../../../MoonlitEngine/Sources/Engine/Component/GameObject.h"

class EditorWindowBase;
class EditorMainWindow;

namespace Moonlit {
	class MoonlitEngine;
}

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

	static Moonlit::Events::Event<void, Moonlit::GameObject*>& OnSelectionChanged()
	{
		static Moonlit::Events::Event<void, Moonlit::GameObject*> instance;
	    return instance;
	}

	MoonlitEditor();

	Moonlit::MoonlitEngine& GetEngine() { return *m_engine; }
	QApplication& GetApplication() { return *m_app; }

private:
	EditorMainWindow* m_mainWindow = nullptr;
	Moonlit::MoonlitEngine* m_engine = nullptr;

	QApplication* m_app = nullptr;
	QTimer* m_updateTimer = nullptr;
	std::function<void()> m_updateCallback;

	std::vector<EditorWindowBase*> m_editorWindows;

	class DefaultDockManager* m_dockManager;

	void LoadDefaultLayout();
};