#include "Editor/Windows/EditorWindowBase.h"

EditorWindowBase::EditorWindowBase(MoonlitEditor* _editor, QWidget* _parent) : QDockWidget(_parent), m_editor(_editor)
{
}
