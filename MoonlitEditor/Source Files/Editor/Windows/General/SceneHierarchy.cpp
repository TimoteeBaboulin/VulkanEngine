#include "Editor/Windows/General/SceneHierarchy.h"

#include <QtWidgets/qtreeview.h>
#include <vector>

#include "Engine/Scene/Scene.h"
#include "Editor/MoonlitEditor.h"
#include "Engine/MoonlitEngine.h"
#include "Engine/GameObject.h"

SceneHierarchy::SceneHierarchy(QWidget* parent)
	: EditorWindowBase(parent)
{
	SetModel();
}

void SceneHierarchy::SetModel()
{
	QTreeView* treeview = new QTreeView();
	Scene& scene = MoonlitEditor::Editor->GetEngine().GetScene();
	std::vector<GameObject*> gameobjects = scene.GetRootGameObjects();
	std::vector<int> indices;
	std::vector<QStandardItem*> currentItems;
	std::vector<QStandardItem*> items;

	for (int i = 0; i < gameobjects.size(); i++)
	{
		indices.push_back(0);
	}

	while (gameobjects.size() > 0)
	{
		GameObject& parent = *gameobjects.back();

		items.push_back(new QStandardItem(parent.GetId()));
		if (currentItems.size() > 0)
		{
			currentItems.back()->appendRow(items.back());
		}
		else
		{
			m_model.appendRow(items.back());
		}
		currentItems.push_back(items.back());

		GameObject* child = gameobjects.back()->GetChildAt(indices.back());
		if (child == nullptr)
		{
			gameobjects.pop_back();
			indices.pop_back();
			currentItems.pop_back();
			continue;
		}
		else
		{
			gameobjects.push_back(child);
			indices.push_back(0);
		}
	}

	treeview->setModel(&m_model);
	setWidget(treeview);
}
