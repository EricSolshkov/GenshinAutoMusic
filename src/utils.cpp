#pragma once
#include "utils.h"

using namespace utils;

utils::Timer::Timer() {}

utils::Timer::~Timer() {}

void utils::Timer::Init() {
	tp = std::chrono::steady_clock::now();
}
void utils::Timer::PreciseSleep(float sec) {
	long interv = long(sec * 1000);
	tp += std::chrono::microseconds(interv);
	std::this_thread::sleep_until(tp);
}

std::string utils::GetAttribute(std::string source, std::string attribute) {
	size_t posPat = source.find(attribute + '=');
	if (posPat != source.npos) {
		size_t posSpace = source.find(' ', posPat);
		size_t count = posSpace - posPat - (attribute.length() + 1);
		std::string attributeValue = source.substr(posPat + (attribute.length() + 1), count);
		if(attributeValue.length()!=0)
			return attributeValue;
	}
	return {};
}

BYTE utils::scan_code(DWORD pKey) {
	const DWORD result = MapVirtualKey(pKey, MAPVK_VK_TO_VSC);
	return static_cast<BYTE>(result);
}

void utils::press_key(DWORD pKey) {
	keybd_event(static_cast<BYTE>(pKey), utils::scan_code(pKey), 0, 0);
}

void utils::release_key(DWORD pKey) {
	keybd_event(static_cast<BYTE>(pKey), utils::scan_code(pKey), KEYEVENTF_KEYUP, 0);
}
