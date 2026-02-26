#include "ProfilerWindow.h"

#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qgridlayout.h>

#include "../../../../../MoonlitEngine/Sources/Debug/Logger.h"

ProfilerWindow::ProfilerWindow(QWidget* _parent) : QWidget(_parent)
{
	SetupUI();
	SetupConnections();
}

//Qt setup methods

void ProfilerWindow::SetupUI()
{
	QGridLayout* layout = new QGridLayout(this);

	m_startProfilingButton = new QPushButton("Start Profiling");
	m_stopProfilingButton = new QPushButton("Stop Profiling");

	m_updateTimer = new QTimer(this);
	m_updateTimer->setInterval(1000/60);
	m_updateTimer->setSingleShot(false);

	m_frameListModel = new ListModel();
	m_frameListModel->listView.setUniformItemSizes(true);
	m_frameListModel->listView.setModel(&m_frameListModel->model);

	//Thread List View
	m_threadListModel = new ListModel();
	m_threadListModel->listView.setUniformItemSizes(true);
	m_threadListModel->listView.setModel(&m_threadListModel->model);

	m_functionTreeView = new QTreeView(this);
	m_functionTreeView->setModel(&m_functionListModel);
	m_functionTreeView->setSortingEnabled(true);

	layout->addWidget(&m_frameListModel->listView, 0, 0, 2, 1);
	layout->addWidget(&m_threadListModel->listView, 2, 0, 2, 1);
	layout->addWidget(m_functionTreeView, 0, 1, 4, 1);

	layout->addWidget(m_startProfilingButton, 4, 0);
	layout->addWidget(m_stopProfilingButton, 4, 1);

	show();
}

void ProfilerWindow::SetupConnections()
{
	connect(m_startProfilingButton, &QPushButton::clicked, this, &ProfilerWindow::StartProfiling);

	connect(m_updateTimer, &QTimer::timeout, this, &ProfilerWindow::ReadProfilerData);

	connect(m_stopProfilingButton, &QPushButton::clicked, this, &ProfilerWindow::StopProfiling);

	connect(&m_frameListModel->listView, &QListView::clicked, this, &ProfilerWindow::PickFrame);
}

void ProfilerWindow::UpdateFrameList(const std::vector<Moonlit::Debug::FrameSample>& _frames)
{
	m_frameListModel->stringList.clear();

	for (const Moonlit::Debug::FrameSample& frame : _frames)
	{
		m_frameListModel->stringList.append(QString::fromStdString(std::to_string(frame.frameId)));
	}

	m_frameListModel->model.setStringList(m_frameListModel->stringList);
	m_currentFrameId = -1;
}

void ProfilerWindow::DisplayFrameSample(const Moonlit::Debug::FrameSample& frameSample)
{
	m_threadListModel->stringList.clear();

	for (auto it = frameSample.stackSamples.begin(); it != frameSample.stackSamples.end(); it++)
	{
		uint64_t threadId = it->first;
		m_threadListModel->stringList.append(QString::fromStdString("Thread ID: " + std::to_string(threadId)));
	}

	m_threadListModel->model.setStringList(m_threadListModel->stringList);
}

void ProfilerWindow::DisplayProfilingSession(const Moonlit::Debug::ProfilingSession& _session)
{
	UpdateFrameList(_session.frameSamples);
	if (!_session.frameSamples.empty())
	{
		DisplayFrameSample(_session.frameSamples[0]);
		m_currentFrameId = _session.frameSamples[0].frameId;
	}

	m_functionListModel.SetFunctions(_session.functionCallCounts);
	m_functionTreeView->setModel(&m_functionListModel);
}

// Qt event callbacks

void ProfilerWindow::ReadProfilerData()
{
	int currentFrameId = Moonlit::Debug::Profiler::Get().GetCurrentFrameId();

	if (currentFrameId == m_currentFrameId)
		return;

	m_profilingSession = Moonlit::Debug::Profiler::Get().GetCurrentSession();

	DisplayProfilingSession(m_profilingSession);
}

void ProfilerWindow::StartProfiling()
{
	LOG_INFO("Starting Profiler...");
	Moonlit::Debug::Profiler::Get().StartProfiling();
	m_updateTimer->start();
}

void ProfilerWindow::StopProfiling()
{
	LOG_INFO("Stopping Profiler...");
	Moonlit::Debug::Profiler::Get().StopProfiling();
	m_updateTimer->stop();
}

void ProfilerWindow::PickFrame(const QModelIndex& _index)
{
	m_currentFrameId = _index.row();
	auto it = std::find_if(m_profilingSession.frameSamples.begin(), m_profilingSession.frameSamples.end(),
		[this](const Moonlit::Debug::FrameSample& frameSample)
		{
			return frameSample.frameId == m_currentFrameId;
		});

	if (it != m_profilingSession.frameSamples.end())
	{
		Moonlit::Debug::FrameSample& frameSample = *it;
		DisplayFrameSample(frameSample);
	}
}

void ProfilerWindow::PickThread(const QModelIndex& _index)
{
}
