
#pragma once
#include <ionCore.h>

class CWorldTime : public Singleton < CWorldTime > {

public:
	void Update();

	void SetTime(std::time_t t);

	std::time_t GetTime() const;
	std::string GetTimeFormatted() const;

	void DecreaseTime(uint seconds);
	void IncreaseTime(uint seconds);

	bool HasTimeChanged() const;

	static uint GetTimeStep();

protected:
	std::time_t WorldTime;
	bool DirtyTime = false;
};