#include "stdafx.h"


/**
 *
 * ChessTimer is a class for high precission function timing
 * with 1 microseconds precission.
 * This class is mainly for DEBUGging purposes only.
 * @Author Olli Koskinen, Arttu Nieminen
 */


/**
 *
 *ChessTimer constructor that initializes all the private variables.
 *@Author Olli Koskinen, Arttu Nieminen
 */
ChessTimer::ChessTimer(){
	PCFreq = 0.0;
	CounterStart = 0;
	total_count = 0;
	total_elapsed = 0;
	changeTimerResolution(MICROSECONDS);
}

ChessTimer::~ChessTimer(){}


/**
 *resetCounter function  that resets all the private variables.
 *
 *@Author Olli Koskinen, Arttu Nieminen
 */
void ChessTimer::resetCounters(){
	PCFreq = 0.0;
	CounterStart = 0;
	total_count = 0;
	total_elapsed = 0;
}

/**
 * StartCounter function for starting the counter. Duh.
 * updates classes internal variables for later use.
 *
 *@Author Olli Koskinen, Arttu Nieminen
 */

void ChessTimer::StartCounter()
{
	LARGE_INTEGER li;
	if(!QueryPerformanceFrequency(&li))
		std::cout << "QueryPerformanceFrequency failed!\n";

	CounterStart = 0;
	PCFreq = double(li.QuadPart)/factor;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
	total_count++;
}

/**
 * Returns the average time
 * 
 *
 *@Author Olli Koskinen, Arttu Nieminen
 */
double ChessTimer::getAverage(){
	return total_count == 0 ? 0 : total_elapsed/total_count;
}


int ChessTimer::getTotalCount(){
	return total_count;
}


/**
 * GetCounter returns the time difference between startCounter and getCounter
 * 
 *@Author Olli Koskinen, Arttu Nieminen
 *@return double the time 
 */
double ChessTimer::GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	double time = double(li.QuadPart-CounterStart)/PCFreq;
	total_elapsed +=time;
	return time;
}


/**
 * changeTimerResolution changes the time resolution acording to the ENUMS passed on to it
 * 
 *@Author Olli Koskinen, Arttu Nieminen
 *@param int resolution
 */
void ChessTimer::changeTimerResolution(int resolution)
{
	switch(resolution){
	case MICROSECONDS:
		factor = 1000000;
		break;
	case MILLISECONDS:
		factor = 1000;
		break;
	case SECONDS:
		factor = 1;
		break;
	default: //If the user somehow gives wrong resolution, we just set it to microseconds
		factor = 1000000;
	}
}
