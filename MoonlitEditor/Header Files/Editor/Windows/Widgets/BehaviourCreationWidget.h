#pragma once

#include <qwidget.h>
#include <qtreeview.h>
#include <qstringlistmodel.h>
#include <qlayout.h>
#include <qscrollarea.h>

#include "Engine/Events/Event.h"

struct BehaviourRegistryEntry;

class BehaviourCreationWidget : public QWidget
{
public:
	BehaviourCreationWidget(class GameObject* _gameObject, QWidget* parent = nullptr);
	void PickBehaviour();
	void SetGameObject(class GameObject* _gameObject) { m_gameObject = _gameObject; }

private:
	std::vector<BehaviourRegistryEntry> m_behaviours;

	QTreeView* m_treeView = nullptr;
	QStringListModel* m_model = nullptr;

	QLayout* m_layout = nullptr;

	QScrollArea* m_scrollArea = nullptr;
	QWidget* m_contentWidget = nullptr;
	QLayout* m_contentLayout = nullptr;

	class GameObject* m_gameObject = nullptr;

	void SetUI();
	void SetupScrollArea();
	void SetupContentWidget();
	void SetupConnections();
};