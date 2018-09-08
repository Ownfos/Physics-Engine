#pragma once
#include <chrono>

using namespace std::chrono;

long long previousTime;
long long currentTime;
long long previousFpsCheck;
long long simulationDuration = 0;
long long accumulator = 0;

long long GetCurrentTime() {
	milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	return ms.count();
}