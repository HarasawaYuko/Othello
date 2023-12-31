#pragma once
#include "include.h"
#include <chrono>

//時間を管理するクラス

class TimeKeeper {
private:
	std::chrono::high_resolution_clock::time_point startTime;
	int64_t time_threshold;
public:
	TimeKeeper();
	TimeKeeper(const int time);
	bool isTimeOver() const;
};