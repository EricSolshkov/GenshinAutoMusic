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

// ���ļ������ֽ���
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
		// ��ɨ��С���ߣ������������ɨ
		if (oriTab[i] == '|') continue;
		Beat beat;
		//TODO������һ��beat��
		// ��ɨ�����Ʒ���������Ʒ����У�����ظ�ֱ����һ���������������š�
		while (symbols::isSymbol(oriTab[i])) {
			beat.ctrl.push_back((CtrlSymbol)oriTab[i]);
			i++;
		}
		// ��ɨ�����ţ�һֱ���ֱ��ɨ�������ţ���Ϊһ����������룬��ɹ��졣
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
		// ��ɨ��������������ɹ���
		else if (symbols::isKey(oriTab[i])) {
			beat.note.push_back((Key)oriTab[i]);
		}
		// ��ɨ����ֹ�����ո�ֱ����ɹ���
		else if (oriTab[i] == '-') {}
		m_tab.push_back(beat);
	}
	std::cout << "Loading Completed" << std::endl;
	return true;
}

// ����Ԥ��������ȫ��ʱֵ����ֵ����Ҫ��ǰ��������������ת���
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

	// Ԥ�ȴ�����β�ģ�����װ��������Խ��
	Beat firstBeat;
	m_tab.insert(m_tab.begin(), firstBeat);
	m_tab.push_back(Beat::Beat());

	//TODO: ������ת��ϵ��ʱֵ��
	(*(m_tab.begin())).next = &(*(m_tab.begin() + 1));
	for (auto beat = m_tab.begin() + 1; beat < m_tab.end() - 1; beat++) {
		(*beat).before = &(*(beat - 1));
		(*beat).next = &(*(beat + 1));
	}
	(*(m_tab.end() - 1)).before = &(*(m_tab.end() - 2));
	//(*(m_tab.end() - 1)).jmp.push_back(fin);
	(*(m_tab.end() - 1)).unfin = false;

	// ������ǰ���Ĵ���
	for (auto beat = m_tab.begin(); beat < m_tab.end(); ++beat) {
		for (auto symbol : (*beat).ctrl) {
			switch (symbol) {
				// ������׼ʱ����
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
			// ������׼ʱ����
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
				// ������������ʱ���������ģ�ָ��������
				if ((*beat).note.size() == 0) {
					(*beat).before->next = (*beat).next;
					(*beat).next->before = (*beat).before;
				}
				break;
			}
			}
		}
	}

	// ��������ʱֵ
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

// ��ͷ��β����
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

		/// �����ٶȵ���
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
		
		// ������ǰ����Ϊ������Ӧ��ʱֵ
		for (char note : beat.deco) {
			press_key(note);
			release_key(note);
			timer.PreciseSleep(currentScale * beat.timeForDeco / ((float)beat.deco.size()));
		}

	}
}

// ������ļ�
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