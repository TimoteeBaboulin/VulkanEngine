#pragma once

#include <QtWidgets/qtreeview.h>

// TODO: switch to QStandardItems
// https://stackoverflow.com/questions/1985936/creating-qt-models-for-tree-views
//class SceneObjectItem
//{
//public:
//	explicit SceneObjectItem(QVariantList _data, SceneObjectItem* _parent);
//
//	void appendChild(std::unique_ptr<SceneObjectItem>&& _child);
//
//	SceneObjectItem* child(int _index);
//	int childCount() const;
//	int columnCount() const;
//	QVariant data(int _column) const;
//	int row() const;
//	SceneObjectItem* parentItem();
//
//private:
//	QVariantList m_dataItem;
//	SceneObjectItem* m_parent;
//	std::vector<std::unique_ptr<SceneObjectItem>> m_children;
//};