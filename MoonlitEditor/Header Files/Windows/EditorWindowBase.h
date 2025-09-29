#pragma once

#include <QtWidgets/qdockwidget.h>

class MoonlitEditor;

class EditorWindowBase : public QDockWidget
{
public:
	EditorWindowBase(MoonlitEditor* _editor);


private:
	MoonlitEditor* m_editor = nullptr;
};