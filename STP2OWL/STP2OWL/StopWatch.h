#pragma once

class StopWatch
{
public:
	StopWatch();
	~StopWatch();

	void Start();
	void Lap();
	void End();

	void ReportTotal() const;

protected:
	void ReportTimeElapsed();

private:
	clock_t m_startTime;
	clock_t m_endTime;
	vector<clock_t> laps;
};