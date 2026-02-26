#include "BehaviourCreationWidget.h"

#include "Engine/Component/BehaviourRegistry.h"
#include "../../MoonlitEditor.h"
#include "../../../../../MoonlitEngine/Sources/Engine/Component/GameObject.h"

bool BehaviourAlreadyExists(const std::string& _behaviourName, Moonlit::GameObject& _gameObject) {
	for (auto behaviour : _gameObject.GetAllBehaviours())
	{
		if (Moonlit::ClassNameFromTypeName(typeid(*behaviour).name()) == _behaviourName)
		{
			return true;
		}
	}

	return false;
}

BehaviourCreationWidget::BehaviourCreationWidget(Moonlit::GameObject* _gameObject, QWidget* parent)
{
	m_behaviours = Moonlit::BehaviourRegistry::GetRegisteredBehaviours();
	m_gameObject = _gameObject;

	SetUI();
}

void BehaviourCreationWidget::PickBehaviour()
{
	QModelIndex index = m_treeView->currentIndex();
	if (!index.isValid()) return;

	std::string behaviourName = m_model->data(index, Qt::DisplayRole).toString().toStdString();
	Moonlit::ObjectBehaviour* behaviour = Moonlit::BehaviourRegistry::CreateBehaviour(behaviourName, m_gameObject);
	MoonlitEditor::OnSelectionChanged().Invoke(nullptr, m_gameObject);
}

void BehaviourCreationWidget::SetGameObject(Moonlit::GameObject *_gameObject) {
	m_gameObject = _gameObject;
	RefreshBehaviours();
}

void BehaviourCreationWidget::RefreshBehaviours() {
	QStringList behaviourNames;
	for (const auto& behaviour : m_behaviours)
	{
		if (!BehaviourAlreadyExists(behaviour.Name, *m_gameObject)) {
			behaviourNames << QString::fromStdString(behaviour.Name);
			LOG_INFO("Added behaviour: " + behaviour.Name);
		}
		else {
			LOG_INFO("Skipped behaviour (already exists): " + behaviour.Name);
		}
	}
	m_model->setStringList(behaviourNames);
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
		if (!BehaviourAlreadyExists(behaviour.Name, *m_gameObject)) {
			behaviourNames << QString::fromStdString(behaviour.Name);
			LOG_INFO("Added behaviour: " + behaviour.Name);
		}
		else {
			LOG_INFO("Skipped behaviour (already exists): " + behaviour.Name);
		}
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
