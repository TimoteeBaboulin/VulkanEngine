#pragma once

#include "Editor/Windows/EditorWindowBase.h"
#include "Editor/ModelClasses/SceneHierarchyModel.h"
#include "Editor/Windows/IDockManager.h"

class SceneHierarchy : public EditorWindowBase
{
public:
	SceneHierarchy(IDockManager* _dockManager);
	SceneHierarchy(QWidget* parent = nullptr);
private:
	void Select(GameObject* selected);

	void SetModel();
	void ContextMenuClicked(QAction* _action);
	void ShowContextMenu(const QPoint& _pos);
	void ShowContextMenu(const QPoint& _pos, GameObject* _obj);

	void OnContextMenuOptionClicked(const QString& _option);

	SceneHierarchyModel* m_model;
	QMenu* m_contextMenu;
	GameObject* m_currentSelected = nullptr;
	GameObject* m_actionTarget = nullptr;
};