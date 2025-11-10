#include "Editor/Models/SceneHierarchyModel.h"
#include "Engine/GameObject.h"

SceneHierarchyModel::SceneHierarchyModel(Scene* scene)
{
	m_scene = scene;

	BuildModel();
}

void SceneHierarchyModel::SetScene(Scene* scene)
{
	m_scene = scene;

	Refresh();
}

void SceneHierarchyModel::Refresh()
{
	beginResetModel();

	BuildModel();

	endResetModel();
}

QModelIndex SceneHierarchyModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
	{
		return QModelIndex();
	}

	if (!parent.isValid())
	{
		if (row < 0 || row >= m_rootObjects.size())
		{
			return QModelIndex();
		}
		return createIndex(row, column, m_rootObjects[row]);
	}

	GameObject* parentObject = static_cast<GameObject*>(parent.internalPointer());
	if (parentObject == nullptr)
	{
		return QModelIndex();
	}

	std::vector<GameObject*> children = parentObject->GetChildren();
	if (row < 0 || row >= children.size())
	{
		return QModelIndex();
	}

	return createIndex(row, column, children[row]);
}

QModelIndex SceneHierarchyModel::parent(const QModelIndex& _index) const
{
	if (!_index.isValid())
	{
		return QModelIndex();
	}

	GameObject* childObject = static_cast<GameObject*>(_index.internalPointer());
	if (!childObject)
	{
		return QModelIndex();
	}

	GameObject* parentObject = childObject->GetParent();
	if (!parentObject)
	{
		return QModelIndex();
	}

	GameObject* grandParentObject = parentObject->GetParent();
	if (!grandParentObject)
	{
		// Parent is a root object
		auto it = std::find(m_rootObjects.begin(), m_rootObjects.end(), parentObject);
		if (it != m_rootObjects.end())
		{
			int row = std::distance(m_rootObjects.begin(), it);
			return createIndex(row, 0, parentObject);
		}

		return QModelIndex();
	}
	else
	{
		auto parentSiblings = grandParentObject->GetChildren();
		auto it = std::find(parentSiblings.begin(), parentSiblings.end(), parentObject);
		if (it != parentSiblings.end())
		{
			int row = std::distance(parentSiblings.begin(), it);
			return createIndex(row, 0, parentObject);
		}
		return QModelIndex();
	}

	return QModelIndex();
}

int SceneHierarchyModel::rowCount(const QModelIndex& parent) const
{
	if (!m_scene)
		return 0;

	if (!parent.isValid())
	{
		return static_cast<int>(m_scene->GetRootGameObjects().size());
	}

	GameObject* parentObject = static_cast<GameObject*>(parent.internalPointer());
	if (!parentObject)
	{
		return 0;
	}

	return parentObject->GetChildren().size();
}

int SceneHierarchyModel::columnCount(const QModelIndex& parent) const
{
	return 1;
}

QVariant SceneHierarchyModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	GameObject* object = static_cast<GameObject*>(index.internalPointer());
	if (!object)
		return QVariant();

	const std::string& name = object->GetName();
	if (name.empty())
		return QString("Error: Unnamed Object");

	return QString::fromStdString(name);
}

QModelIndex SceneHierarchyModel::GetIndexForGameObject(GameObject* object) const
{
	if (!object)
		return QModelIndex();

	if (object->GetParent())
	{
		//Not a root object
		GameObject* parentObj = object->GetParent();
		auto siblings = parentObj->GetChildren();
		auto it = std::find(siblings.begin(), siblings.end(), object);
		if (it == siblings.end())
			return QModelIndex();

		int row = std::distance(siblings.begin(), it);
		return createIndex(row, 0, object);
	}

	auto itRoot = std::find(m_rootObjects.begin(), m_rootObjects.end(), object);
	if (itRoot == m_rootObjects.end())
		return QModelIndex();

	int row = std::distance(m_rootObjects.begin(), itRoot);
	return createIndex(row, 0, object);
}

void SceneHierarchyModel::BuildModel()
{
	m_rootObjects.clear();
	if (!m_scene)
	{
		return;
	}

	m_rootObjects = m_scene->GetRootGameObjects();
}
