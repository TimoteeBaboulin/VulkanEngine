#pragma once

#include <QtCore/qabstractitemmodel.h>

#include "Engine/Scene/Scene.h"

class SceneHierarchyModel : public QAbstractListModel
{
public:
	SceneHierarchyModel(Scene* scener);

	void SetScene(Scene* scene);
	void Refresh();

	QModelIndex index(int row, int column = 0, const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex& index) const override;

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
	QModelIndex GetIndexForGameObject(class GameObject* object) const;
	void BuildModel();

	std::vector<class GameObject*> m_rootObjects;

	Scene* m_scene;
};