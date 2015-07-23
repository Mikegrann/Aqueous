#include "CWorldTime.h"

#define TIME_STEP 300

void CWorldTime::Update() {
	DirtyTime = false;
}

void CWorldTime::SetTime(std::time_t t) {
	WorldTime = t;
	DirtyTime = true;
}

std::time_t CWorldTime::GetTime() const
{
	return WorldTime;
}

std::string CWorldTime::GetTimeFormatted() const
{
	std::string retVal;
	stringstream ss(retVal);
	std::tm *timeOBJ = std::localtime(&WorldTime);

	ss << std::put_time(timeOBJ, "%Y-%m-%d %H:%M:%S");

	return ss.str();
}

bool CWorldTime::HasTimeChanged() const {
	return DirtyTime;
}

void CWorldTime::DecreaseTime(uint seconds) {
	SetTime(WorldTime - seconds);
}

void CWorldTime::IncreaseTime(uint seconds) {
	SetTime(WorldTime + seconds);
}

uint CWorldTime::GetTimeStep() {
	return TIME_STEP;
}