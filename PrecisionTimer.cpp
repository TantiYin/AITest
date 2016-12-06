#include "PrecisionTimer.h"

CTimer::CTimer() : mStartTime(0), mLastTime(0)
{
	INT64 Frequency = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&Frequency);
	mFrequency = 1.0f / Frequency;
}

void CTimer::Start()
{
	INT64 CurrentTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime);
	mStartTime = CurrentTime;
	mLastTime = mStartTime;
}

float CTimer::Update()
{
	INT64 CurrentTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime);
	float DeltaTime = 0;
	DeltaTime = (CurrentTime - mLastTime) * mFrequency;
	mLastTime = CurrentTime;
	return DeltaTime;
}


