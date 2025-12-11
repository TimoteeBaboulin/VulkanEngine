#include "Editor/Windows/General/SceneHierarchy.h"

#include <QtWidgets/qtreeview.h>
#include <vector>

#include "Engine/Scene/Scene.h"
#include "Editor/MoonlitEditor.h"
#include "Engine/MoonlitEngine.h"
#include "Engine/GameObject.h"

#include <qboxlayout.h>
#include <qmenu.h>

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

	treeview->connect(treeview, &QTreeView::clicked, this, [this, treeview]() {
		auto selected = treeview->currentIndex();
		GameObject* obj = static_cast<GameObject*>(selected.internalPointer());
		MoonlitEditor::OnSelectionChanged().Invoke(this, obj);
		});

	treeview->setContextMenuPolicy(Qt::CustomContextMenu);
	treeview->connect(treeview, &QTreeView::customContextMenuRequested, this, &SceneHierarchy::ShowContextMenu);
}

void SceneHierarchy::ContextMenuClicked(QAction* _action)
{
	if (_action->text() == "Create Empty GameObject")
	{
		GameObject* newObj = GameObject::CreateAt(glm::vec3(0.0f));
		MoonlitEngine::GetInstance()->GetScene().AddGameObject(newObj);
		newObj->SetName("New GameObject");
		m_model->Refresh();
		m_model->layoutChanged();
	}

	m_contextMenu->deleteLater();
}

void SceneHierarchy::ShowContextMenu(const QPoint& _pos)
{
	m_contextMenu = new QMenu(this);
	m_contextMenu->addAction("Create Empty GameObject");
	m_contextMenu->connect(m_contextMenu, &QMenu::triggered, this, &SceneHierarchy::ContextMenuClicked);
	m_contextMenu->popup(mapToGlobal(_pos));
}
