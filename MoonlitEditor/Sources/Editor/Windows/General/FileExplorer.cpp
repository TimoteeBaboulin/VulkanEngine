#include "FileExplorer.h"
#include "../../MoonlitEditor.h"
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qboxlayout.h>


//FileExplorer::FileExplorer(QWidget* _parent)
//	: EditorWindowBase(_parent)
//{
//	Init();
//}

FileExplorer::FileExplorer(IDockManager* _dockManager)
	: EditorWindowBase()
{
	_dockManager->AddWidget(this, "File Explorer", ads::BottomDockWidgetArea);
	Init();
}

FileExplorer::~FileExplorer()
{
	delete m_fileSystemModel;
}

void FileExplorer::Init()
{
	m_fileSystemModel = new QFileSystemModel(this);
	QString path = MoonlitEditor::Editor->GetApplication().applicationDirPath();
	m_fileSystemModel->setRootPath(path);

	m_treeView = new QTreeView(this);
	m_treeView->setModel(m_fileSystemModel);
	m_treeView->setRootIndex(m_fileSystemModel->index(path));

	m_treeView->setColumnHidden(1, true); // Size
	m_treeView->setColumnHidden(2, true); // Type
	m_treeView->setColumnHidden(3, true); // Date

	QLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom);
	layout->addWidget(m_treeView);
	setLayout(layout);
	//setWidget(m_treeView);

	show();
}
