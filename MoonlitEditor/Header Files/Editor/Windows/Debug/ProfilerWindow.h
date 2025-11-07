#pragma once

#include <QtWidgets/qwidget.h>
#include <QtCore/qtimer.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlistview.h>
#include <QtWidgets/qtreeview.h>
#include <QtCore/qstringlistmodel.h>

#include "Debug/Profiler.h"
#include "Editor/Models/MethodViewModel.h"

struct ListModel
{
	QStringListModel model;
	QStringList stringList;
	QListView listView;
};

class  ProfilerWindow : public QWidget
{
public:
	ProfilerWindow(QWidget* _parent = nullptr);

private:
	//Qt setup methods

	void SetupUI();
	void SetupConnections();

	void UpdateFrameList(const std::vector<FrameSample>& _frames);
	void DisplayFrameSample(const FrameSample& frameSample);
	void DisplayProfilingSession(const ProfilingSession& _session);

	//Qt event callbacks

	void ReadProfilerData();
	void StartProfiling();
	void StopProfiling();

	void PickFrame(const QModelIndex& _index);
	void PickThread(const QModelIndex& _index);

	// The profiler window uses its own timer
	// This is used to avoid coupling the profiler with the rest
	// of the editor update loop
	
	QTimer* m_updateTimer = nullptr;
	QPushButton* m_startProfilingButton = nullptr; 
	QPushButton* m_stopProfilingButton = nullptr;

	ListModel* m_frameListModel = nullptr;
	ListModel* m_threadListModel = nullptr;

	QTreeView* m_functionTreeView = nullptr;

	ProfilingSession m_profilingSession;
	MethodListModel m_functionListModel;

	int m_currentFrameId = -1;
};