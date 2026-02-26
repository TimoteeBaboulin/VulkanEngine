#include "SceneHierarchy.h"

#include <QtWidgets/qtreeview.h>
#include <vector>

#include "../../../../../MoonlitEngine/Sources/Engine/Scene/Scene.h"
#include "../../MoonlitEditor.h"
#include "../../../../../MoonlitEngine/Sources/Engine/MoonlitEngine.h"
#include "../../../../../MoonlitEngine/Sources/Engine/Component/GameObject.h"

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

void SceneHierarchy::Select(GameObject *selected) {
	if (m_currentSelected == selected) return;

	m_currentSelected = selected;
	MoonlitEditor::OnSelectionChanged().Invoke(this, selected);
}

void SceneHierarchy::SetModel()
{
	QTreeView* treeview = new QTreeView(this);
	Moonlit::Scene& scene = MoonlitEditor::Editor->GetEngine().GetScene();

	m_model = new SceneHierarchyModel(&scene);
	treeview->setModel(m_model);

	QLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom);
	layout->addWidget(treeview);
	setLayout(layout);

	treeview->connect(treeview, &QTreeView::clicked, this, [this, treeview]() {
		auto selected = treeview->currentIndex();
		GameObject* obj = static_cast<GameObject*>(selected.internalPointer());
		Select(obj);
		});

	treeview->connect(treeview, &QTreeView::customContextMenuRequested, this, [this, treeview](const QPoint& _pos) {
		auto selected = treeview->indexAt(_pos);
		GameObject* obj = static_cast<GameObject*>(selected.internalPointer());
		if (!obj) {
			ShowContextMenu(_pos);
			return;
		}

		ShowContextMenu(_pos, obj);
		});

	treeview->setContextMenuPolicy(Qt::CustomContextMenu);
}

void SceneHierarchy::ContextMenuClicked(QAction* _action)
{
	std::string actionText = _action->text().toStdString();
	if (actionText == "Create Empty GameObject")
	{
		GameObject* newObj = GameObject::CreateAt(glm::vec3(0.0f));
		Moonlit::MoonlitEngine::GetInstance()->GetScene().AddGameObject(newObj);
		newObj->SetName("New GameObject");
		m_model->Refresh();
		m_model->layoutChanged();
	}
	else if (actionText == "Delete")
	{
		if (m_actionTarget)
		{
			GameObject::Destroy(*m_currentSelected);
			m_model->Refresh();
			m_model->layoutChanged();
		}
	}
	else if (actionText == "Create Empty Child") {
		if (m_actionTarget)
		{
			GameObject* newObj = GameObject::CreateAt(glm::vec3(0.0f));
			m_currentSelected->AddChild(newObj);
			newObj->SetName("New Child");
			m_model->Refresh();
			m_model->layoutChanged();
		}
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

void SceneHierarchy::ShowContextMenu(const QPoint &_pos, GameObject *_obj) {
	m_contextMenu = new QMenu(this);
	m_contextMenu->addAction("Delete");
	m_contextMenu->addAction("Create Empty Child");
	m_actionTarget = _obj;
	m_contextMenu->connect(m_contextMenu, &QMenu::triggered, this, &SceneHierarchy::ContextMenuClicked);
	m_contextMenu->popup(mapToGlobal(_pos));
}