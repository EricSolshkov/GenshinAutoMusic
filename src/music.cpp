#include "music.h"
using namespace music;

Beat::Beat() {
	jmp = {}; ctrl = {}; note = {}; deco = {}; timeForDeco = 0.0; before = nullptr; next = nullptr;
}

Beat::~Beat() {}

Tab::Tab() {
	timeSig = "44";
	BPM = 90;
	defaultNote = oct;
	defaultBeatTime = (float)60000 / BPM;
	m_tab = {};
}

Tab::Tab(std::string GLBtimeSig, float GLBbpm, char GLBdefaultNote) {
	timeSig = GLBtimeSig;
	BPM = GLBbpm;
	defaultNote = (CtrlSymbol)GLBdefaultNote;
	defaultBeatTime = (float)60000 / BPM;
	m_tab = {};
}

Tab::~Tab() {};

// 从文件加载字节流
bool Tab::Load(char* filename) {
	m_tab.clear();
	std::ifstream fin;
	if (filename != nullptr) {
		fin.open(filename);
		if (!fin) {
			std::cout << "Error in Loading.\n";
			abort();
		}
	}
	else return false;

	std::string oriTab = {};
	std::string temp;
	while (!fin.eof()) {
		getline(fin, temp);
		if (temp[0] == '#') {
			
			std::string tempTimeSig = utils::GetAttribute(temp, "TimeSig");
			timeSig = tempTimeSig == "" ? timeSig : tempTimeSig;

			std::string tempBPM = utils::GetAttribute(temp,"BPM");
			float sum = 0, scale = 1.0;
			bool afterDot = false;
			for (auto i = tempBPM.begin(); i != tempBPM.end(); ++i) {
				if (*i == '.') afterDot = true;
				if (*i >= 48 && *i <= 57) {
					sum = sum * 10.0 + (float)(((WCHAR)*i) - 48);
					if (afterDot)
						scale *= 10.0;
				}
			}
			sum = sum / scale;
			BPM = sum == 0 ? BPM : sum;

			std::string tempDefaultNote = utils::GetAttribute(temp, "DefaultNote");
			if (tempDefaultNote != "" && symbols::isSymbol(tempDefaultNote[0])) {
				defaultNote = (CtrlSymbol)tempDefaultNote[0];
			}
			temp.clear();
		}
		oriTab += temp;
	}

	for (unsigned int i = 0; i < oriTab.size(); i++)
	{
		// 若扫到小节线，不作处理，向后扫
		if (oriTab[i] == '|') continue;
		Beat beat;
		//TODO：构造一个beat；
		// 若扫到控制符，加入控制符队列，向后重复直到第一个音符，或者括号。
		while (symbols::isSymbol(oriTab[i])) {
			beat.ctrl.push_back((CtrlSymbol)oriTab[i]);
			i++;
		}
		// 若扫到括号，一直向后直到扫到后括号，作为一个音符组加入，完成构造。
		if (oriTab[i] == '(') {
			i++;
			while (oriTab[i] != ')') {
				if (symbols::isSymbol(oriTab[i]))
					beat.ctrl.push_back((CtrlSymbol)oriTab[i]);
				else if (symbols::isKey(oriTab[i])) {
					beat.note.push_back((Key)oriTab[i]);
				}
				i++;
			}
		}
		// 若扫到音符，加入完成构造
		else if (symbols::isKey(oriTab[i])) {
			beat.note.push_back((Key)oriTab[i]);
		}
		// 若扫到休止符、空格，直接完成构造
		else if (oriTab[i] == '-') {}
		m_tab.push_back(beat);
	}
	std::cout << "Loading Completed" << std::endl;
	return true;
}

// 乐谱预处理，处理全局时值基础值、需要提前发声的音符、跳转标记
void Tab::PreProcess() {
	switch (timeSig[0]) {
	case '2': {
		time[whole] = 120000.0 / BPM;
		break;
	}
	case '4': {
		time[whole] = 240000.0 / BPM;
		break;
	}
	case '8': {
		time[whole] = 480000.0 / BPM;
		break;
	}
	case 'p': {
		time[whole] = 960000.0 / BPM;
		break;
	}
	}
	time[half] = time[whole] / 2.0;
	time[quad] = time[half] / 2.0;
	time[oct] = time[quad] / 2.0;
	time[hex] = time[oct] / 2.0;
	defaultBeatTime = time[defaultNote];

	// 预先创建首尾拍，以免装饰音访问越界
	Beat firstBeat;
	m_tab.insert(m_tab.begin(), firstBeat);
	m_tab.push_back(Beat::Beat());

	//TODO: 创建跳转关系、时值；
	(*(m_tab.begin())).next = &(*(m_tab.begin() + 1));
	for (auto beat = m_tab.begin() + 1; beat < m_tab.end() - 1; beat++) {
		(*beat).before = &(*(beat - 1));
		(*beat).next = &(*(beat + 1));
	}
	(*(m_tab.end() - 1)).before = &(*(m_tab.end() - 2));
	//(*(m_tab.end() - 1)).jmp.push_back(fin);
	(*(m_tab.end() - 1)).unfin = false;

	// 含有提前音的处理
	for (auto beat = m_tab.begin(); beat < m_tab.end(); ++beat) {
		for (auto symbol : (*beat).ctrl) {
			switch (symbol) {
				// 琶音：准时结束
			case arpgInTime: {
				if ((*beat).before == nullptr) {
					(*(beat - 1)).deco = (*beat).note;
					std::vector<Key> tempNote((*beat).note.end() - 1, (*beat).note.end());
					(*beat).note = tempNote;
					(*(beat - 1)).deco.erase((*(beat - 1)).deco.end() - 1);
					(*(beat - 1)).timeForDeco = time[hex] / 2;
				}
				else {
					(*((*beat).before)).deco = (*beat).note;
					std::vector<Key> tempNote((*beat).note.end() - 1, (*beat).note.end());
					(*beat).note = tempNote;
					(*((*beat).before)).deco.erase((*((*beat).before)).deco.end() - 1);
					(*((*beat).before)).timeForDeco = time[hex] / 2;
				}
				break;
			}
			// 倚音：准时结束
			case appoggiatura: {
				if ((*beat).before == nullptr) {
					(*(beat - 1)).deco = { (*beat).note[0] };
					(*beat).note.erase((*beat).note.begin());
					(*(beat - 1)).timeForDeco = time[hex] / 3;
				}
				else {
					(*((*beat).before)).deco = { (*beat).note[0] };
					(*beat).note.erase((*beat).note.begin());
					(*((*beat).before)).timeForDeco = time[hex] / 3;
				}
				// 倚音单独成拍时，消掉此拍（指针跳过）
				if ((*beat).note.size() == 0) {
					(*beat).before->next = (*beat).next;
					(*beat).next->before = (*beat).before;
				}
				break;
			}
			}
		}
	}

	// 调整各拍时值
	for (auto beat = m_tab.begin(); beat < m_tab.end(); ++beat) {
		(*beat).duration = defaultBeatTime;
		for (auto symbol : (*beat).ctrl) {
			if (symbol == whole || symbol == half || symbol == quad || symbol == oct || symbol == hex) {
				(*beat).duration = time[symbol];
			}
		}
		for (auto symbol : (*beat).ctrl) {
			switch (symbol) {
			case dot: {
				(*beat).duration *= 1.5;
				break;
			}
			case triplet: {
				(*beat).duration /= 3.0;
				break;
			}
			case quintuplet: {
				(*beat).duration /= 5.0;
				break;
			}
			case septuplet: {
				(*beat).duration /= 7.0;
				break;
			}
			}
		}
		(*beat).duration -= (*beat).timeForDeco;
	}
	return;
}

// 从头至尾播放
void Tab::Play() {
	timer.Init();
	for (auto beat = *(m_tab.begin()); beat.unfin; beat = *(beat.next)) {
		for (char note : beat.note) {
			if (symbols::isKey(note)) {
				press_key(note);
				release_key(note);
				//printf("%c", note);
			}
		}

		/// 渐进速度调整
		bool isRit = false, isAccel = false;
		for (auto symbol : beat.ctrl) {
			switch (symbol) {
			case rit: {
				isRit = true;
				currentScale *= std::pow(1.2, beat.duration / defaultBeatTime);
				currentScale = currentScale > 1.5 ? 1.5 : currentScale;
				break;
			}
			case accel: {
				currentScale *= std::pow(0.8, beat.duration / defaultBeatTime);
				currentScale = currentScale > 0.75 ? currentScale : 0.75;
				break;
			}
			default: {
				//isRit = false, isAccel = false;
				break;
			}
			}
		}
		if (!isRit && !isAccel) currentScale = 1.0;

		timer.PreciseSleep(beat.duration * currentScale);
		
		// 若有提前音，为其分配对应的时值
		for (char note : beat.deco) {
			press_key(note);
			release_key(note);
			timer.PreciseSleep(currentScale * beat.timeForDeco / ((float)beat.deco.size()));
		}

	}
}

// 输出到文件
void Tab::Output(std::string name) {
	std::ofstream fout;
	if (name.length() == 0) {
		name = "Untitled";
	}
	fout.open(name + ".txt");
	float time = 0;
	for (auto beat : m_tab) {
		for (auto symbol : beat.ctrl)
			fout << symbol;
		if (beat.note.size() == 0)
			fout << '-';
		else
		{
			fout << '(';
			for (auto note : beat.note) {
				fout << note;
			}
			fout << ')';
		}
	}
}