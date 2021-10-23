// GenshinAutoMusic.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <Windows.h>
#include <vector>
#include <string>
#include "utils.h"
#include "music.h"

using namespace music;
using namespace utils;

int main(int argc, char* argv[]) {

	Tab theTab;
	if (!theTab.Load(argv[1])) return 0;
	theTab.PreProcess();
	Timer timer0;

	timer0.Init();
	timer0.PreciseSleep(6000);
	theTab.Play();
	
	return 0;
}