#include "Debug/Profiler.h"
#include "Debug/Logger.h"

#include <string>


Profiler& Profiler::Get()
{
	static Profiler instance;
	return instance;
}

Profiler::Profiler()
{
	DWORD symOptions = SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES | SYMOPT_NO_PROMPTS;
	SymSetOptions(symOptions);
	if (!SymInitialize(GetCurrentProcess(), NULL, TRUE))
	{
		LOG_ERROR("Profiler::Profiler: Failed to initialize symbol handler.");
	}

	std::lock_guard<std::mutex> lock(m_runningMutex);
	m_running = false;
	//m_profilerThreadId = GetCurrentThreadId();
}

void Profiler::StartProfiling()
{
	std::lock_guard<std::mutex> lock(m_runningMutex);
	if (!m_running)
	{
		m_running = true;
		m_profilingThread = std::thread(&Profiler::Profile, this);
	}
}

void Profiler::StopProfiling()
{
	if (m_running)
	{
		m_runningMutex.lock();
		m_running = false;
		m_runningMutex.unlock();

		if (m_profilingThread.joinable())
		{
			m_profilingThread.join();
		}
	}
}

void Profiler::SetProfileInterval(double _intervalMs)
{
	if (_intervalMs <= 0)
	{
		LOG_WARNING("Profiler::SetProfileInterval: Interval given is 0 or less, resetting to default value.");
		_intervalMs = DEFAULT_PROFILE_INTERVAL_MS;
	}

	std::lock_guard<std::mutex> lock(m_runningMutex);
	m_profileIntervalMs = _intervalMs;
}

FrameSample Profiler::GetLastFrameSample()
{
	std::lock_guard<std::mutex> lock(m_runningMutex);
	if (m_lastFrameSampled == nullptr)
	{
		LOG_WARNING("Profiler::GetLastFrameSample: No frame has been sampled yet.");
		return FrameSample();
	}
	return *m_lastFrameSampled;
}

ProfilingSession Profiler::GetCurrentSession()
{
	std::lock_guard<std::mutex> lock(m_runningMutex);
	return m_currentSession;
}

uint64_t Profiler::GetCurrentFrameId()
{
	std::lock_guard<std::mutex> lock(m_runningMutex);
	return m_currentFrameId;
}

void Profiler::BuildStackFrame(LPSTACKFRAME _frame, LPDWORD _imageType, CONTEXT& _ctx)
{
#ifdef _M_IX86
	_frame->AddrPC.Offset = ctx.Eip;
	_frame->AddrPC.Mode = AddrModeFlat;
	_frame->AddrFrame.Offset = _ctx.Ebp;
	_frame->AddrFrame.Mode = AddrModeFlat;
	_frame->AddrStack.Offset = _ctx.Esp;
	_frame->AddrStack.Mode = AddrModeFlat;
	*_imageType = IMAGE_FILE_MACHINE_I386;
#elif _M_X64
	_frame->AddrPC.Offset = _ctx.Rip;
	_frame->AddrPC.Mode = AddrModeFlat;
	_frame->AddrFrame.Offset = _ctx.Rbp;
	_frame->AddrFrame.Mode = AddrModeFlat;
	_frame->AddrStack.Offset = _ctx.Rsp;
	_frame->AddrStack.Mode = AddrModeFlat;
	*_imageType = IMAGE_FILE_MACHINE_AMD64;
#elif _M_ARM64
	_frame->AddrPC.Offset = _ctx.Pc;
	_frame->AddrPC.Mode = AddrModeFlat;
	_frame->AddrFrame.Offset = _ctx.Pp;
	_frame->AddrFrame.Mode = AddrModeFlat;
	_frame->AddrStack.Offset = ctx.Sp;
	_frame->AddrStack.Mode = AddrModeFlat;
	*_imageType = IMAGE_FILE_MACHINE_ARM64;
#else
#error "Platform not supported!"
#endif
}

std::vector<ThreadAddressSample> Profiler::GetStackAddresses()
{
	DWORD processId = GetCurrentProcessId();
	DWORD threadId = GetCurrentThreadId();

	std::vector<ThreadAddressSample> stackAddresses;

	CONTEXT ctx = {  };
	ctx.ContextFlags = CONTEXT_FULL;

	// Create the snapshot to get all currently running threads
	if (!processId || !threadId)
	{
		LOG_ERROR("Profiler::GetProfileSample: Invalid process or thread ID.");
		return stackAddresses;
	}

	HANDLE h_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (h_snapshot == INVALID_HANDLE_VALUE)
	{
		LOG_ERROR("Profiler::GetProfileSample: Failed to create thread snapshot.");
		return stackAddresses;
	}

	THREADENTRY32 threadEntry = {};
	threadEntry.dwSize = sizeof(threadEntry);
	if (!Thread32First(h_snapshot, &threadEntry))
	{
		CloseHandle(h_snapshot);
		DWORD error = GetLastError();
		LOG_ERROR("Profiler::GetProfileSample: Failed to get first thread in snapshot. Error: " + std::to_string(error));
		return stackAddresses;
	}

	// Iterate over every thread from our process except for our own
	do
	{
		if (threadEntry.th32OwnerProcessID != processId)
		{
			continue;
		}

		if (threadEntry.th32ThreadID == threadId)
		{
			continue;
		}

		stackAddresses.push_back(ThreadAddressSample());
		ThreadAddressSample& threadSample = stackAddresses.back();

		HANDLE h_thread = OpenThread(THREAD_ALL_ACCESS, FALSE, threadEntry.th32ThreadID);
		if (!h_thread)
		{
			LOG_ERROR("Profiler::GetProfileSample: Failed to open thread with ID " + std::to_string(threadEntry.th32ThreadID) + ".");
			continue;
		}

		BOOL result = SuspendThread(h_thread);
		if (result == (DWORD)-1)
		{
			LOG_ERROR("Profiler::GetProfileSample: Failed to suspend thread with ID " + std::to_string(threadEntry.th32ThreadID) + ".");
			CloseHandle(h_thread);
			continue;
		}

		GetThreadContext(h_thread, &ctx);
		result = ResumeThread(h_thread);
		if (result == (DWORD)-1)
		{
			LOG_ERROR("Profiler::GetProfileSample: Failed to resume thread with ID " + std::to_string(threadEntry.th32ThreadID) + ".");
			CloseHandle(h_thread);
			continue;
		}

		CloseHandle(h_thread);

		// Initialize stack frame
		STACKFRAME64 stackFrame = {};
		DWORD imageType = 0;

		BuildStackFrame(&stackFrame, &imageType, ctx);

		HANDLE h_process = GetCurrentProcess();
		if (!h_process)
		{
			LOG_ERROR("Profiler::GetProfileSample: Failed to get current process handle.");
			continue;
		}

		threadSample.threadId = threadEntry.th32ThreadID;

		while (StackWalk(imageType,
			h_process,
			h_thread,
			&stackFrame,
			&ctx,
			NULL,
			SymFunctionTableAccess,
			SymGetModuleBase,
			NULL
		))
		{
			if (stackFrame.AddrPC.Offset == 0)
			{
				break;
			}

			threadSample.stackAddresses.push(stackFrame.AddrPC.Offset);
		}
	} while (Thread32Next(h_snapshot, &threadEntry));

	return stackAddresses;
}

FrameSample Profiler::ProfileFrame()
{
	FrameSample sample;
	std::vector<ThreadAddressSample> stackAddresses = GetStackAddresses();
	HANDLE processHandle = GetCurrentProcess();
	if (processHandle == NULL)
	{
		LOG_ERROR("Profiler::ProfileFrame: Failed to get current process handle.");
		return sample;
	}

	for (auto it = stackAddresses.begin(); it != stackAddresses.end(); it++)
	{
		ThreadAddressSample& threadSample = *it;

		while (!threadSample.stackAddresses.empty())
		{
			uint64_t address = threadSample.stackAddresses.front();
			threadSample.stackAddresses.pop();

			// Resolve address to symbol
			char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)] = {  };
			PSYMBOL_INFO symbol = (PSYMBOL_INFO)buffer;
			symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
			symbol->MaxNameLen = MAX_SYM_NAME;
			if (SymFromAddr(processHandle, address, 0, symbol))
			{
				StackSampleItem item;
				item.functionName = symbol->Name;
				sample.stackSamples[threadSample.threadId].push_back(StackSampleItem{symbol->Name});
			}
		}
	}

	return sample;
}

void Profiler::Profile()
{
	double intervalMs = 0.0;
	while (true)
	{
		// Use lock guard to avoid forgetting to unlock the mutex
		{
			std::lock_guard<std::mutex> lock(m_runningMutex);

			if (!m_running)
			{
				return;
			}

			intervalMs = m_profileIntervalMs;
		}

		FrameSample frame = ProfileFrame();
		frame.frameId = m_currentFrameId++;

		std::map<std::string, int> functionCallCounts;
		for (auto it = frame.stackSamples.begin(); it != frame.stackSamples.end(); it++)
		{
			const std::vector<StackSampleItem>& stackItems = it->second;
			for (const StackSampleItem& item : stackItems)
			{
				functionCallCounts[item.functionName]++;
			}
		}
		

		m_runningMutex.lock();
		m_currentSession.frameSamples.push_back(frame);
		m_lastFrameSampled = &m_currentSession.frameSamples.back();

		for (auto it = functionCallCounts.begin(); it != functionCallCounts.end(); it++)
		{
			m_currentSession.functionCallCounts[it->first] += it->second;
		}
		m_runningMutex.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(intervalMs)));
	}
}
