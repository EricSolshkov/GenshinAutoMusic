#pragma once
#include "utils.h"

using namespace utils;

utils::Timer::Timer() {}

utils::Timer::~Timer() {}

void utils::Timer::Init() {
	tp = std::chrono::steady_clock::now();
	initTime = tp;
}
void utils::Timer::PreciseSleep(float milisec) {
	long interv = long(milisec * 1000);
	tp += std::chrono::microseconds(interv);
	std::this_thread::sleep_until(tp);
}
std::chrono::steady_clock::time_point utils::Timer::PreciseTime() {
	return std::chrono::steady_clock::now();
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




void utils::press_key(INPUT key) {
	SendInput(1, &key, sizeof(INPUT));
	key.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &key, sizeof(INPUT));
}

void utils::press_key(char key) {
	INPUT ikey;
	ZeroMemory(&ikey,sizeof(ikey));
	ikey.type = INPUT_KEYBOARD;
	ikey.ki.wVk = key;
	SendInput(1, &ikey, sizeof(INPUT));
	ikey.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ikey, sizeof(INPUT));
}

	
