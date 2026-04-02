#pragma once

#include <qtreeview.h>

#include "Editor/Windows/EditorWindowBase.h"
#include "Editor/ModelClasses/SceneHierarchyModel.h"
#include "Editor/Windows/IDockManager.h"
#include "Engine/Events/EventUtility.h"

class SceneHierarchy : public EditorWindowBase
{
public:
	SceneHierarchy(IDockManager* _dockManager);
	SceneHierarchy(QWidget* parent = nullptr);
private:
	void Select(GameObject* selected);

	void SetUi();
	void SetModel();
	void OnSceneLoaded(Moonlit::Scene* _scene);
	void ContextMenuClicked(QAction* _action);
	void ShowContextMenu(const QPoint& _pos);
	void ShowContextMenu(const QPoint& _pos, GameObject* _obj);

	void OnContextMenuOptionClicked(const QString& _option);

	SceneHierarchyModel* m_model = nullptr;
	QMenu* m_contextMenu;
	QTreeView* m_treeView;

	GameObject* m_currentSelected = nullptr;
	GameObject* m_actionTarget = nullptr;
	Moonlit::Events::ScopedEventSubscriber<Moonlit::Scene*> m_sceneLoadedSubscriber;
};
