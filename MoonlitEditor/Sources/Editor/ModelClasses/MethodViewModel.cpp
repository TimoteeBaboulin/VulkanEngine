#include "MethodViewModel.h"

QModelIndex MethodListModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();
	return createIndex(row, column);
}

QModelIndex MethodListModel::parent(const QModelIndex& index) const
{
	// This model is a flat list, so it has no parent

	return QModelIndex();
}

int MethodListModel::rowCount(const QModelIndex& parent) const
{
	return m_functionCallCounts.size();
}

int MethodListModel::columnCount(const QModelIndex& parent) const
{
	// This model has two columns: Function Name and Call Count
	return 2;
}

QVariant MethodListModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid() || index.row() >= m_functionCallCounts.size() || index.column() >= 2)
		return QVariant();

	QVariant value;

	// This model only supports display role as value is read-only
	if (role == Qt::DisplayRole)
	{
		std::pair<std::string, int> pair;
		pair = *std::next(m_functionCallCounts.begin(), index.row());
		if (index.column() == 0)
		{
			value = QString::fromStdString(pair.first);
		}
		else if (index.column() == 1)
		{
			value = pair.second;
		}
	}

	return value;
}

void MethodListModel::SetFunctions(const std::map<std::string, int>& functions)
{
	beginResetModel();
	for (const auto& func : functions)
	{
		int callCount = func.second;
		std::string functionName = func.first;
		if (m_functionCallCounts.find(functionName) == m_functionCallCounts.end())
		{
			m_functionCallCounts.insert(std::make_pair(functionName, 0));
		}

		m_functionCallCounts[functionName] = callCount;
	}
	endResetModel();
}
