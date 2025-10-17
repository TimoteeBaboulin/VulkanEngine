#pragma once

#include <QtWidgets/qmainwindow.h>

class  EditorMainWindow : public QMainWindow
{
public:
	EditorMainWindow(size_t _width, size_t _height);
	bool event(QEvent* _event) override;

	static bool TryGetInstance(EditorMainWindow*& _window);

private:
	static EditorMainWindow* m_instance;

	size_t m_width, m_height;
};