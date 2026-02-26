#include "ObjectEditor.h"

#include "../../../../../MoonlitEngine/Sources/Engine/Component/GameObject.h"
#include "../../MoonlitEditor.h"

#include <QBoxLayout>
#include <QScrollArea>
#include <QVBoxLayout>

ObjectInspector::ObjectInspector(IDockManager& _manager)
	: EditorWindowBase(nullptr)
	, m_selectionChangedSubscriber(MoonlitEditor::OnSelectionChanged(), std::bind(&ObjectInspector::OnSelectedObjectChanged, this, std::placeholders::_1))
{
	m_newBehaviourWidget = nullptr;
	_manager.AddWidget(this, "Object Inspector", ads::DockWidgetArea::RightDockWidgetArea);
	SetUI();
}

void ObjectInspector::OnSelectedObjectChanged(Moonlit::GameObject* _obj)
{
	// remove existing widgets from the content layout and delete them
	for (auto widget : m_behaviourWidgets)
	{
		if (m_contentLayout)
		{
			m_contentLayout->removeWidget(widget);
		}
		delete widget;
	}
	m_behaviourWidgets.clear();

	if (m_newBehaviourWidget)
	{
		m_contentLayout->removeWidget(m_newBehaviourWidget);
		m_newBehaviourWidget->SetGameObject(_obj);
	}
	else
	{
		m_newBehaviourWidget = new BehaviourCreationWidget(_obj, m_contentWidget);
	}

	if (!_obj)
		return;

	std::vector<Moonlit::ObjectBehaviour*> behaviours = _obj->GetAllBehaviours();

	// Add new behaviour widgets to the content layout
	for (auto& behaviour : behaviours)
	{
		BehaviourEditor* bw = new BehaviourEditor(behaviour);
		m_behaviourWidgets.push_back(bw);
		if (m_contentLayout)
			m_contentLayout->addWidget(bw);
	}

	if (m_contentLayout)
		m_contentLayout->addWidget(m_newBehaviourWidget);
	// optional: push a stretch at the end to keep them at the top if needed
	// (not required if alignment is Qt::AlignTop and the content widget height fits)
	// static_cast<QVBoxLayout*>(m_contentLayout)->addStretch();

	// ensure UI updates
	m_contentWidget->adjustSize();
}

void ObjectInspector::SetUI()
{
	// Top-level layout
	auto mainLayout = new QVBoxLayout();
	mainLayout->setSpacing(10);
	mainLayout->setContentsMargins(10, 10, 10, 10);
	m_layout = mainLayout;

	// Scroll area
	m_scrollArea = new QScrollArea(this);
	m_scrollArea->setWidgetResizable(true);
	m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	// Content widget inside the scroll area
	m_contentWidget = new QWidget();
	auto contentLayout = new QVBoxLayout();
	contentLayout->setSpacing(6);
	contentLayout->setContentsMargins(4, 4, 4, 4);
	contentLayout->setAlignment(Qt::AlignTop); // <- important: force stacking from the top
	m_contentWidget->setLayout(contentLayout);
	m_contentLayout = contentLayout;

	m_scrollArea->setWidget(m_contentWidget);

	// Add scroll area to the main layout
	mainLayout->addWidget(m_scrollArea);

	setLayout(mainLayout);
}
