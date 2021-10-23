#pragma once
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include "utils.h"
#include "Symbols.h"

using namespace symbols;
using namespace utils;

namespace music {

	struct Beat {
		std::vector<symbols::CtrlSymbol> ctrl;
		std::vector<symbols::Key> note;
		std::vector<symbols::Key> deco;
		std::vector<INPUT> note_input;
		std::vector<INPUT> deco_input;
		std::vector<symbols::JmpSymbol> jmp;

		float timeForDeco, duration;
		bool unfin = true;
		Beat* before = nullptr, * next = nullptr;
		
		Beat();
		~Beat();
	};

	class Tab {
	private:
		std::vector<Beat> m_tab;
		float BPM, defaultBeatTime, currentScale = 1.0;
		symbols::CtrlSymbol defaultNote;
		std::string timeSig;
		std::map<symbols::CtrlSymbol, float > time;
		std::vector<float> timeline = {};
	public:
		Timer timer;
		Tab();
		Tab(std::string GLBtimeSig, float GLBbpm, char GLBdefaultNote);
		~Tab();

		bool Load(char* filename);
		void PreProcess();	
		void Play();
		void Output(std::string name);
	};
}