#pragma once

#include "Editor/Windows/EditorWindowBase.h"
#include "Editor/Windows/IDockManager.h"

#include "Engine/Events/EventUtility.h"
#include "Editor/Windows/Widgets/BehaviourWidget.h"

#include <qscrollarea.h>

class ObjectInspector : public EditorWindowBase
{
public:
	ObjectInspector(IDockManager& _manager);

private:
	void OnSelectedObjectChanged(class GameObject* _obj);

	void SetUI();

	ScopedEventSubscriber<class GameObject*> m_selectionChangedSubscriber;
	QLayout* m_layout = nullptr;

	// scroll area + content widget that actually contains the behaviour widgets
	QScrollArea* m_scrollArea = nullptr;
	QWidget* m_contentWidget = nullptr;
	QLayout* m_contentLayout = nullptr;

	// store behaviour widgets by pointer (owned and deleted by this inspector)
	std::vector<BehaviourWidget*> m_behaviourWidgets;
};