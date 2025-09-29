#pragma once

#include <vector>
#include <functional>

class EditorWindowBase;
class EditorMainWindow;
class MoonlitEngine;

class QApplication;
class QTimer;

class MoonlitEditor
{
public:
	MoonlitEditor();

private:
	EditorMainWindow* m_mainWindow = nullptr;
	MoonlitEngine* m_engine = nullptr;

	QApplication* m_app = nullptr;
	QTimer* m_updateTimer = nullptr;
	std::function<void()> m_updateCallback;

	std::vector<EditorWindowBase*> m_editorWindows;
};