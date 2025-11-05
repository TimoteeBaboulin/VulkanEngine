#pragma once

#include "Editor/Windows/EditorWindowBase.h"

#include <QtGui/qstandarditemmodel.h>


class SceneHierarchy : public EditorWindowBase
{
public:
	SceneHierarchy(QWidget* parent = nullptr);
private:
	void SetModel();

	QStandardItemModel m_model;
};