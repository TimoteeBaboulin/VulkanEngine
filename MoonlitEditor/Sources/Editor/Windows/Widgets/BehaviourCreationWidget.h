#pragma once

#include <qwidget.h>
#include <qtreeview.h>
#include <qstringlistmodel.h>
#include <qlayout.h>
#include <qscrollarea.h>

#include "Engine/Events/Event.h"

namespace Moonlit
{
	class GameObject;

	struct BehaviourRegistryEntry;
}

class BehaviourCreationWidget : public QWidget
{
public:
	BehaviourCreationWidget(Moonlit::GameObject* _gameObject, QWidget* parent = nullptr);
	void PickBehaviour();
	void SetGameObject(Moonlit::GameObject* _gameObject);
	void RefreshBehaviours();

private:
	std::vector<Moonlit::BehaviourRegistryEntry> m_behaviours;

	QTreeView* m_treeView = nullptr;
	QStringListModel* m_model = nullptr;

	QLayout* m_layout = nullptr;

	QScrollArea* m_scrollArea = nullptr;
	QWidget* m_contentWidget = nullptr;
	QLayout* m_contentLayout = nullptr;

	Moonlit::GameObject* m_gameObject = nullptr;

	void SetUI();
	void SetupScrollArea();
	void SetupContentWidget();
	void SetupConnections();
};