#pragma once

#include <QtCore/qabstractitemmodel.h>
#include <QtGui/qstandarditemmodel.h>

#include "Engine/Scene/Scene.h"
#include "Engine/Component/GameObject.h"

using GameObject = Moonlit::GameObject;

class SceneHierarchyModel : public QAbstractListModel
{
public:
	SceneHierarchyModel(Moonlit::Scene* _scene);

	void SetScene(Moonlit::Scene* _scene);
	void Refresh();

	QModelIndex index(int row, int column = 0, const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex& index) const override;

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
	QModelIndex GetIndexForGameObject(GameObject* object) const;
	void BuildModel();
	void CountRows(GameObject* _parent, int& count) const;

	std::vector<GameObject*> m_rootObjects;

	Moonlit::Scene* m_scene;
};