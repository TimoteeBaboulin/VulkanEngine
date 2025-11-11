#include "Editor/Windows/General/SceneHierarchy.h"

#include <QtWidgets/qtreeview.h>
#include <vector>

#include "Engine/Scene/Scene.h"
#include "Editor/MoonlitEditor.h"
#include "Engine/MoonlitEngine.h"
#include "Engine/GameObject.h"

#include <qboxlayout.h>

SceneHierarchy::SceneHierarchy(IDockManager* _dockManager)
	: EditorWindowBase()
{
	_dockManager->AddWidget(this, "Scene Hierarchy", ads::LeftDockWidgetArea);
	SetModel();
}

SceneHierarchy::SceneHierarchy(QWidget* parent)
	: EditorWindowBase(parent)
{
	SetModel();
}

void SceneHierarchy::SetModel()
{
	QTreeView* treeview = new QTreeView(this);
	Scene& scene = MoonlitEditor::Editor->GetEngine().GetScene();

	m_model = new SceneHierarchyModel(&scene);
	treeview->setModel(m_model);

	QLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom);
	layout->addWidget(treeview);
	setLayout(layout);
	//setWidget(treeview);
}
