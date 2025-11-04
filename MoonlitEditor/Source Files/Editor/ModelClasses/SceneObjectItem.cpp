#include "Editor/ModelClasses/SceneObjectItem.h"

SceneObjectItem::SceneObjectItem(QVariantList _data, SceneObjectItem* _parent)
	: m_dataItem(std::move(_data)), m_parent(_parent)
{
	
}

void SceneObjectItem::appendChild(std::unique_ptr<SceneObjectItem>&& _child)
{
	m_children.push_back(_child);
}

SceneObjectItem* SceneObjectItem::child(int _index)
{
	if (_index < 0 || _index > m_children.size())
		return nullptr;
	return m_children.at(_index).get();
}

int SceneObjectItem::childCount() const
{
	return m_children.size();
}

int SceneObjectItem::columnCount() const
{
	return m_dataItem.size();
}

QVariant SceneObjectItem::data(int _column) const
{
	return m_dataItem.value(_column);
}

int SceneObjectItem::row() const
{
	if (m_parent == nullptr)
		return 0;
	const auto it = std::find_if(m_parent->m_children.cbegin(), m_parent->m_children.cend(),
		[this](const std::unique_ptr<SceneObjectItem>& treeItem) {
			return treeItem.get() == this;
		});

	if (it != m_parent->m_children.cend())
		return std::distance(m_parent->m_children.cbegin(), it);
	Q_ASSERT(false); // should not happen
	return -1;
}

SceneObjectItem* SceneObjectItem::parentItem()
{
	return m_parent;
}