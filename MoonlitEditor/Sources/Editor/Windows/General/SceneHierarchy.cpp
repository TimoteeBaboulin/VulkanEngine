#include "SceneHierarchy.h"

#include "Engine/Scene/Scene.h"
#include "Editor/MoonlitEditor.h"
#include "Engine/MoonlitEngine.h"
#include "Engine/Component/GameObject.h"

#include <QtWidgets/qtreeview.h>
#include <qboxlayout.h>
#include <qmenu.h>

#include "Engine/Scene/SceneManager.h"

SceneHierarchy::SceneHierarchy(IDockManager *_dockManager)
    : EditorWindowBase(), m_sceneLoadedSubscriber(Moonlit::MoonlitEngine::GetInstance()->OnSceneLoaded, std::bind(&SceneHierarchy::OnSceneLoaded, this, std::placeholders::_1))
{
    _dockManager->AddWidget(this, "Scene Hierarchy", ads::LeftDockWidgetArea);
    SetUi();
    SetModel();
}

SceneHierarchy::SceneHierarchy(QWidget *parent)
    : EditorWindowBase(parent), m_sceneLoadedSubscriber(Moonlit::MoonlitEngine::GetInstance()->OnSceneLoaded, std::bind(&SceneHierarchy::OnSceneLoaded, this, std::placeholders::_1))
{
    SetUi();
    SetModel();
}

void SceneHierarchy::Select(GameObject *selected)
{
    if (m_currentSelected == selected) return;

    m_currentSelected = selected;
    MoonlitEditor::OnSelectionChanged()(this, selected);
}

void SceneHierarchy::SetUi()
{
    m_treeView = new QTreeView(this);

    QLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom);
    layout->addWidget(m_treeView);
    setLayout(layout);

    m_treeView->connect(m_treeView, &QTreeView::clicked, this, [this]() {
        auto selected = m_treeView->currentIndex();
        GameObject *obj = static_cast<GameObject *>(selected.internalPointer());
        Select(obj);
    });

    m_treeView->connect(m_treeView, &QTreeView::customContextMenuRequested, this, [this](const QPoint &_pos) {
        auto selected = m_treeView->indexAt(_pos);
        GameObject *obj = static_cast<GameObject *>(selected.internalPointer());
        if (!obj)
        {
            ShowContextMenu(_pos);
            return;
        }

        ShowContextMenu(_pos, obj);
    });

    m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);
}

void SceneHierarchy::SetModel()
{
    Moonlit::Scene &scene = MoonlitEditor::Editor->GetEngine().GetScene();

    if (m_model)
    {
        delete m_model;
    }

    m_model = new SceneHierarchyModel(&scene);
    m_treeView->setModel(m_model);
    m_currentSelected = nullptr;

    std::vector<GameObject*> rootObjects = scene.GetRootGameObjects();
    if (rootObjects.size() > 0) {
        Select(rootObjects[0]);
    }
}

void SceneHierarchy::OnSceneLoaded(Moonlit::Scene *_scene) {
    SetModel();
}

void SceneHierarchy::ContextMenuClicked(QAction *_action)
{
    std::string actionText = _action->text().toStdString();
    if (actionText == "Create Empty GameObject")
    {
        GameObject *newObj = GameObject::CreateAt(glm::vec3(0.0f));
        Moonlit::MoonlitEngine::GetInstance()->GetScene().AddGameObject(newObj);
        newObj->SetName("New GameObject");
        m_model->Refresh();
        m_model->layoutChanged();
    }
    else if (actionText == "Delete")
    {
        if (m_actionTarget)
        {
            if (m_actionTarget == m_currentSelected)
            {
                m_currentSelected = nullptr;
                MoonlitEditor::OnSelectionChanged()(this, m_currentSelected);
            }

            GameObject::Destroy(*m_actionTarget);
            m_model->Refresh();
            m_model->layoutChanged();
        }
    }
    else if (actionText == "Create Empty Child")
    {
        if (m_actionTarget)
        {
            GameObject *newObj = GameObject::CreateAt(glm::vec3(0.0f));
            m_currentSelected->AddChild(newObj);
            newObj->SetName("New Child");
            m_model->Refresh();
            m_model->layoutChanged();
        }
    }
    else if (actionText == "Save Scene")
    {
        Moonlit::MoonlitEngine::GetInstance()->GetScene().Save("scene.ms");
    }

    m_contextMenu->deleteLater();
}

void SceneHierarchy::ShowContextMenu(const QPoint &_pos)
{
    m_contextMenu = new QMenu(this);
    m_contextMenu->addAction("Create Empty GameObject");
    m_contextMenu->addAction("Save Scene");
    m_contextMenu->connect(m_contextMenu, &QMenu::triggered, this, &SceneHierarchy::ContextMenuClicked);
    m_contextMenu->popup(mapToGlobal(_pos));
}

void SceneHierarchy::ShowContextMenu(const QPoint &_pos, GameObject *_obj)
{
    m_contextMenu = new QMenu(this);
    m_contextMenu->addAction("Delete");
    m_contextMenu->addAction("Create Empty Child");
    m_actionTarget = _obj;
    m_contextMenu->connect(m_contextMenu, &QMenu::triggered, this, &SceneHierarchy::ContextMenuClicked);
    m_contextMenu->popup(mapToGlobal(_pos));
}