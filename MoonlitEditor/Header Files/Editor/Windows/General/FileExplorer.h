#pragma once

#include "Editor/Windows/EditorWindowBase.h"

#include <QtWidgets/qfilesystemmodel.h>
#include <QtWidgets/qtreeview.h>

class FileExplorer : public EditorWindowBase
{
public:
    FileExplorer() = delete;
    FileExplorer(QWidget* _parent = nullptr);
    ~FileExplorer();
private:
	void Init();
	QFileSystemModel* m_fileSystemModel = nullptr;
    QTreeView* m_treeView = nullptr;
};
