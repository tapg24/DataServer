#ifndef TIMER_H
#define TIMER_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class CXTimer
{
	__int64 Freq_;
	__int64 Start_;

	void InitFreq()
	{
		QueryPerformanceFrequency((PLARGE_INTEGER) &Freq_);
	}

public:
	CXTimer()
	{
		InitFreq();
		Reset();
	}

	void Reset()
	{
		QueryPerformanceCounter((PLARGE_INTEGER) &Start_);
	}

	inline double GetElapsed()
	{
		__int64 Now;
		QueryPerformanceCounter((PLARGE_INTEGER) &Now);
		return (double) (Now - Start_);
	}

	inline double GetElapsedSec()
	{
		return GetElapsed() / GetFreq();
	}

	inline double GetFreq()
	{
		return (double) Freq_;
	}
};

#endif // TIMER_H