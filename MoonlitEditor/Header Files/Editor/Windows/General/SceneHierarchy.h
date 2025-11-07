#pragma once

#include "Editor/Windows/EditorWindowBase.h"

#include "Editor/Models/SceneHierarchyModel.h"


class SceneHierarchy : public EditorWindowBase
{
public:
	SceneHierarchy(QWidget* parent = nullptr);
private:
	void SetModel();

	SceneHierarchyModel* m_model;
};