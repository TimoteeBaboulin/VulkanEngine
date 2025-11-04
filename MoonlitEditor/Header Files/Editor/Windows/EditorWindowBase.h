#pragma once

#include <QtWidgets/qdockwidget.h>

class EditorWindowBase : public QDockWidget
{
public:
	EditorWindowBase(QWidget* _parent = nullptr);
};