#pragma once

#include <QtCore/qabstractitemmodel.h>

class MethodListModel : public QAbstractListModel
{
public:
	QModelIndex index(int row, int column = 0, const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex& index) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	void SetFunctions(const std::map<std::string, int>& functions);
private:
	std::map<std::string, int> m_functionCallCounts;
};