#pragma once
#include <Windows.h>
#include <vector>
#include <cstdint>
#include <string>
#include <chrono>
#include <Thread>

namespace utils {

	class Timer {
	public:
		std::chrono::steady_clock::time_point tp;
		std::chrono::steady_clock::time_point initTime;
		Timer();
		~Timer();
		void Init();
		void PreciseSleep(float sec);
		std::chrono::steady_clock::time_point PreciseTime();
	};
	
	std::string GetAttribute(std::string source, std::string attribute);
	
	
	void press_key(INPUT key);
	void press_key(char key);
}