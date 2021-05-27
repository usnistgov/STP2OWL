#include "stdafx.h"
#include "StopWatch.h"

StopWatch::StopWatch()
	: m_startTime(0.0),
	m_endTime(0.0)
{
}

StopWatch::~StopWatch()
{
}

void StopWatch::Start()
{
	m_startTime = clock();
}

void StopWatch::Lap()
{
	End();
	Start();
}

void StopWatch::End()
{
	m_endTime = clock();
	ReportTimeElapsed();
}

void StopWatch::ReportTotal() const
{
	clock_t totalTime = accumulate(laps.begin(), laps.end(), 0);
	double timeInSec = totalTime / (double)CLOCKS_PER_SEC;

	cout << "Total time elapsed: " << timeInSec << " seconds" << endl << endl;
}

void StopWatch::ReportTimeElapsed()
{
	clock_t netTime = m_endTime - m_startTime;
	double timeInSec = netTime / (double)CLOCKS_PER_SEC;

	cout << "Time elapsed: " << timeInSec << " seconds" << endl << endl;
	laps.push_back(netTime);
}
