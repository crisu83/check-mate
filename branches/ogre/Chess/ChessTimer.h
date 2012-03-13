#pragma once

enum{
	MICROSECONDS = 0,
	MILLISECONDS, SECONDS
};

class ChessTimer
{
private:
	double PCFreq;
	__int64 CounterStart;
	int total_count;
	double total_elapsed;
	int factor;

public:
	ChessTimer();
	~ChessTimer();
	void StartCounter();
	double GetCounter();
	double getAverage();
	int getTotalCount();
	void resetCounters();
	void changeTimerResolution(int resolution);
};