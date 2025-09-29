#include "Windows/EditorMainWindow.h"

EditorMainWindow* EditorMainWindow::m_instance = nullptr;

EditorMainWindow::EditorMainWindow(size_t _width, size_t _height) : QMainWindow(), m_width(_width), m_height(_height)
{
    //Having multiple Main Windows could mean 2 editor classes are created for the same process.
    //This is not currently allowed as this might cause issues later down the line.
    if (m_instance != nullptr)
    {
		throw std::runtime_error("An instance of EditorMainWindow already exists.");
    }

    m_instance = this;

    setMinimumHeight(_height);
    setMaximumHeight(_height);
    setMinimumWidth(_width);
    setMaximumWidth(_width);

	//Make sure to resize the window to the specified width and height.
	resize(_width, _height);

    show();
}

bool EditorMainWindow::TryGetInstance(EditorMainWindow*& _window)
{
    if (m_instance == nullptr)
    {
        return false;
    }

	_window = m_instance;
    return true;
}
