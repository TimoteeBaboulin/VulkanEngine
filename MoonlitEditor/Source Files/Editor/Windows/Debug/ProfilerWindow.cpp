#include "Editor/Windows/Debug/ProfilerWindow.h"

#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qgridlayout.h>

#include "Debug/Logger.h"

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

	layout->addWidget(&m_frameListModel->listView, 0, 0, 3, 2);
	layout->addWidget(m_startProfilingButton, 3, 0);
	layout->addWidget(m_stopProfilingButton, 3, 1);
	layout->addWidget(&m_threadListModel->listView, 0, 2, 3, 1);

	show();
}

void ProfilerWindow::SetupConnections()
{
	connect(m_startProfilingButton, &QPushButton::clicked, this, &ProfilerWindow::StartProfiling);

	connect(m_updateTimer, &QTimer::timeout, this, &ProfilerWindow::ReadProfilerData);

	connect(m_stopProfilingButton, &QPushButton::clicked, this, &ProfilerWindow::StopProfiling);

	connect(&m_frameListModel->listView, &QListView::clicked, this, &ProfilerWindow::PickFrame);
}

void ProfilerWindow::UpdateFrameList(const std::vector<FrameSample>& _frames)
{
	m_frameListModel->stringList.clear();

	for (const FrameSample& frame : _frames)
	{
		m_frameListModel->stringList.append(QString::fromStdString(std::to_string(frame.frameId)));
	}

	m_frameListModel->model.setStringList(m_frameListModel->stringList);
	m_currentFrameId = -1;
}

void ProfilerWindow::DisplayFrameSample(const FrameSample& frameSample)
{
	m_threadListModel->stringList.clear();

	for (auto it = frameSample.stackSamples.begin(); it != frameSample.stackSamples.end(); it++)
	{
		uint64_t threadId = it->first;
		m_threadListModel->stringList.append(QString::fromStdString("Thread ID: " + std::to_string(threadId)));
	}

	m_threadListModel->model.setStringList(m_threadListModel->stringList);
}

void ProfilerWindow::DisplayProfilingSession(const ProfilingSession& _session)
{
	UpdateFrameList(_session.frameSamples);
	if (!_session.frameSamples.empty())
	{
		DisplayFrameSample(_session.frameSamples[0]);
		m_currentFrameId = _session.frameSamples[0].frameId;
	}
}

// Qt event callbacks

void ProfilerWindow::ReadProfilerData()
{
	int currentFrameId = Profiler::Get().GetCurrentFrameId();

	if (currentFrameId == m_currentFrameId)
		return;

	m_profilingSession = Profiler::Get().GetCurrentSession();

	DisplayProfilingSession(m_profilingSession);
}

void ProfilerWindow::StartProfiling()
{
	LOG_INFO("Starting Profiler...");
	Profiler::Get().StartProfiling();
	m_updateTimer->start();
}

void ProfilerWindow::StopProfiling()
{
	LOG_INFO("Stopping Profiler...");
	Profiler::Get().StopProfiling();
	m_updateTimer->stop();
}

void ProfilerWindow::PickFrame(const QModelIndex& _index)
{
	m_currentFrameId = _index.row();
	auto it = std::find_if(m_profilingSession.frameSamples.begin(), m_profilingSession.frameSamples.end(),
		[this](const FrameSample& frameSample)
		{
			return frameSample.frameId == m_currentFrameId;
		});

	if (it != m_profilingSession.frameSamples.end())
	{
		FrameSample& frameSample = *it;
		DisplayFrameSample(frameSample);
	}
}

void ProfilerWindow::PickThread(const QModelIndex& _index)
{
}
