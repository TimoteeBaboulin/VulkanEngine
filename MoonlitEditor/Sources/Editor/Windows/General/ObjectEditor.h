#pragma once

#include "Editor/Windows/EditorWindowBase.h"
#include "Editor/Windows/IDockManager.h"
#include "Editor/Widgets/BehaviourWidget.h"
#include "Editor/Widgets/BehaviourCreationWidget.h"
#include "Engine/Events/EventUtility.h"

#include <qscrollarea.h>

namespace Moonlit
{
	class GameObject;
}

class ObjectInspector : public EditorWindowBase
{
public:
	ObjectInspector(IDockManager& _manager);

private:
	void OnSelectedObjectChanged(Moonlit::GameObject* _obj);

	void ResetUI();
	void Clear();

	void SetUI();

	Moonlit::Events::ScopedEventSubscriber<Moonlit::GameObject* > m_selectionChangedSubscriber;
	QLayout* m_layout = nullptr;

	// scroll area + content widget that actually contains the behaviour widgets
	QScrollArea* m_scrollArea = nullptr;
	QWidget* m_contentWidget = nullptr;
	QLayout* m_contentLayout = nullptr;

	// store behaviour widgets by pointer (owned and deleted by this inspector)
	std::vector<BehaviourEditor*> m_behaviourWidgets;
	BehaviourCreationWidget* m_newBehaviourWidget = nullptr;
};