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

	m_model = new SceneHierarchyModel(&scene);
	treeview->setModel(m_model);
	setWidget(treeview);
}
