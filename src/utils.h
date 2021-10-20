#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>
#include <chrono>
#include <Thread>

namespace utils {

	class Timer {
	public:
		std::chrono::steady_clock::time_point tp;
		Timer();
		~Timer();
		void Init();
		void PreciseSleep(float sec);
	};
	
	std::string GetAttribute(std::string source, std::string attribute);
	BYTE scan_code(DWORD pKey);
	void press_key(DWORD pKey);
	void release_key(DWORD pKey);

}