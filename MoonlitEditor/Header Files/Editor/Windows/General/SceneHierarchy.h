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

	SceneHierarchyModel* m_model;
};