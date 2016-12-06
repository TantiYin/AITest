#pragma once

#include <windows.h>

class CTimer
{
public:
	CTimer();
	~CTimer() {}

	void Start();

	float Update();

private:
	INT64 mStartTime, mLastTime;
	float mFrequency;
};