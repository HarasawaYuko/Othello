#include "TimeKeeper.h"

TimeKeeper::TimeKeeper() {

}

TimeKeeper::TimeKeeper(const int time) 
	:startTime(std::chrono::high_resolution_clock::now()),time_threshold(time)
{}

bool TimeKeeper::isTimeOver() const {
	auto diff = std::chrono::high_resolution_clock::now() - startTime;
	return std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() >= time_threshold;
}