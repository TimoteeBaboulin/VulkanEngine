#pragma once

#include "Editor/Windows/EditorWindowBase.h"

#include "Editor/Models/SceneHierarchyModel.h"
#include "Editor/Windows/IDockManager.h"


class SceneHierarchy : public EditorWindowBase
{
public:
	SceneHierarchy(IDockManager* _dockManager);
	SceneHierarchy(QWidget* parent = nullptr);
private:
	void SetModel();
	void ContextMenuClicked(QAction* _action);
	void ShowContextMenu(const QPoint& _pos);

	SceneHierarchyModel* m_model;
	QMenu* m_contextMenu;
};