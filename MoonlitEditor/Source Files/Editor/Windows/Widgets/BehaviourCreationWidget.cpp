#include "Editor/Windows/Widgets/BehaviourCreationWidget.h"

#include "Engine/Components/BehaviourRegistry.h"
#include "Editor/MoonlitEditor.h"
#include "Engine/GameObject.h"

BehaviourCreationWidget::BehaviourCreationWidget(GameObject* _gameObject, QWidget* parent)
{
	m_behaviours = BehaviourRegistry::GetRegisteredBehaviours();
	m_gameObject = _gameObject;

	SetUI();
}

void BehaviourCreationWidget::PickBehaviour()
{
	QModelIndex index = m_treeView->currentIndex();
	if (!index.isValid()) return;

	std::string behaviourName = m_model->data(index, Qt::DisplayRole).toString().toStdString();
	ObjectBehaviour* behaviour = BehaviourRegistry::CreateBehaviour(behaviourName, m_gameObject);
	MoonlitEditor::OnSelectionChanged().Invoke(nullptr, m_gameObject);
}

void BehaviourCreationWidget::SetUI()
{
	m_layout = new QVBoxLayout(this);
	setLayout(m_layout);

	m_treeView = new QTreeView(this);

	m_model = new QStringListModel(this);

	QStringList behaviourNames;
	for (const auto& behaviour : m_behaviours)
	{
		behaviourNames << QString::fromStdString(behaviour.Name);
	}
	m_model->setStringList(behaviourNames);

	m_treeView->setModel(m_model);

	SetupConnections();
	SetupContentWidget();
	SetupScrollArea();

	show();
}

void BehaviourCreationWidget::SetupScrollArea()
{
	m_scrollArea = new QScrollArea(this);
	m_scrollArea->setWidgetResizable(true);
	m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_scrollArea->setWidget(m_contentWidget);

	m_layout->addWidget(m_scrollArea);
}

void BehaviourCreationWidget::SetupContentWidget()
{
	m_contentWidget = new QWidget();
	m_contentLayout = new QVBoxLayout(m_contentWidget);
	m_contentLayout->addWidget(m_treeView);
}

void BehaviourCreationWidget::SetupConnections()
{
	m_treeView->connect(m_treeView, &QTreeView::clicked, [this]() {PickBehaviour(); });
}
