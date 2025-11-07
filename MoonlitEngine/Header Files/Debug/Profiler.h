#pragma once

#include <thread>
#include <mutex>
#include <queue>
#include <map>

#include <Windows.h>
#include <TlHelp32.h>
//Include dbghelp AFTER Windows.h
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")

#include "MoonlitExport.h"

struct StackSampleItem
{
	std::string functionName;
};

struct MOONLIT_API FrameSample
{
	int frameId;
	std::map<uint64_t, std::vector<StackSampleItem>> stackSamples;
};

struct MOONLIT_API ProfilingSession
{
	double sessionInterval;
	std::vector<FrameSample> frameSamples;

	std::map<std::string, int> functionCallCounts;
};

struct ThreadAddressSample
{
	DWORD threadId;
	std::queue<uint64_t> stackAddresses;
};

constexpr int DEFAULT_PROFILE_INTERVAL_MS = 250;

class MOONLIT_API Profiler
{
public:
	static Profiler& Get();

public:
	Profiler();

	void StartProfiling();
	void StopProfiling();

	void SetProfileInterval(double _intervalMs);
	FrameSample GetLastFrameSample();
	ProfilingSession GetCurrentSession();
	uint64_t GetCurrentFrameId();
private:
	std::thread m_profilingThread;
	uint64_t m_currentFrameId = 0;

	bool m_running = false;
	double m_profileIntervalMs = DEFAULT_PROFILE_INTERVAL_MS;

	std::mutex m_runningMutex;
	FrameSample* m_lastFrameSampled = nullptr;
	ProfilingSession m_currentSession;

	void BuildStackFrame(LPSTACKFRAME _frame, LPDWORD _imageType, CONTEXT& _ctx);
	std::vector<ThreadAddressSample> GetStackAddresses();
	FrameSample ProfileFrame();

	void Profile();
};