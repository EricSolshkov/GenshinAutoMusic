#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>
#include <chrono>
#include <Thread>

std::chrono::steady_clock::time_point tp;

void PreciseSleepInit() {
	tp = std::chrono::steady_clock::now();
}

void PreciseSleep(float sec) {
	long interv = long(sec*1000);
	//tp = std::chrono::steady_clock::now();
	tp += std::chrono::microseconds(interv);
	std::this_thread::sleep_until(tp);
}

BYTE scan_code(DWORD pKey) {
	const DWORD result = MapVirtualKey(pKey, MAPVK_VK_TO_VSC);
	return static_cast<BYTE>(result);
}

void press_key(DWORD pKey) {
	keybd_event(static_cast<BYTE>(pKey), scan_code(pKey), 0, 0);
}

void release_key(DWORD pKey) {
	keybd_event(static_cast<BYTE>(pKey), scan_code(pKey), KEYEVENTF_KEYUP, 0);
}
	