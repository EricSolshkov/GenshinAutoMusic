// GenshinAutoMusic.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <vector>
#include <string>

BYTE scan_code(DWORD pKey)
{
    const DWORD result = MapVirtualKey(pKey, MAPVK_VK_TO_VSC);
    return static_cast<BYTE>(result);
}
void press_key(DWORD pKey)
{
    keybd_event(static_cast<BYTE>(pKey), scan_code(pKey), 0, 0);
}
void release_key(DWORD pKey)
{
    keybd_event(static_cast<BYTE>(pKey), scan_code(pKey), KEYEVENTF_KEYUP, 0);
}

class Tab {
private:
    std::vector<std::vector<char>> m_tab;
    float BPM;
    std::string timeSig;
public:
    
    Tab(std::string GLBtimeSig,float GLBbpm) {
        timeSig = GLBtimeSig;
        BPM = GLBbpm;
    }
    void Load() {
        m_tab.clear();
        std::ifstream fin;
        fin.open("Tab.txt");
        if (!fin){
            std::cout << "Error...\n";
            abort();
        }
        std::string oriTab = {};
        std::string temp;
        while (!fin.eof()){
            getline(fin, temp);
            oriTab+=temp;
        }
        for (int i = 0; i < oriTab.size(); i++)
        {
            if (oriTab[i] == '-' || oriTab[i] == ' ') {
                m_tab.push_back({});
            }
            else if (oriTab[i] == '(') {
                i++;
                std::vector<char> beat;
                while (oriTab[i] != ')') {
                    beat.push_back(oriTab[i]);
                    i++;
                }
                m_tab.push_back(beat);
            }
            else if (oriTab[i] == '|') {
                //Do Nothing;
            }
            else {
                m_tab.push_back({ oriTab[i] });
            }
                
        }
        std::cout << "Loading Completed"<<std::endl;
    }

    void Output(std::string name) {
        std::ofstream fout;
        fout.open(name+".txt");
        float time = 0;
        for (auto beat : m_tab) {
            if (beat.size() == 0) {
                fout << '-';
            }
            else if (beat.size() == 1) {
                fout << beat[0];
            }
            else {
                fout << '(';
                for (auto note : beat) {
                    fout << note;
                }
                fout << ')';
                
                /* 统计音符时值
                switch (int(beat[0] - 48)) {
                // 1 全音符
                case 1: {
                    time += (timeSig[0] - 48) * TIMEPERBEAT;
                    break;
                    }
                // 2 二分音符
                case 2: {
                    time += (timeSig[0] - 48) * TIMEPERBEAT / 2;
                    break;
                }
                // 4 二分音符
                case 4: {
                    time += (timeSig[0] - 48) * TIMEPERBEAT / 4;
                    break;
                }
                // 8 二分音符
                case 8: {
                    time += (timeSig[0] - 48) * TIMEPERBEAT / 8;
                    break;
                }
                
                // -2(46) 附点(一拍的附点)
                case 46: {
                    time += 3 * TIMEPERBEAT / 2;
                    break;
                }
                // 3 三连音
                case 3: {
                    time += TIMEPERBEAT / 3;
                    break;
                }
                }*/   
            }
        }
            
            /*画小节线
            if (beatSerial == timeSig[1]-48) {
                beatSerial = 0;
                fout << '|';
            }*/
    }
    void Play() {
        for (auto beat : m_tab) {
            for (auto note : beat) {
                if (note != '-' && note != '3') {
                    press_key(note);
                    release_key(note);
                    std::cout << note;
                }
            }

            std::cout << ' ';
            
            if (beat.size()!=0 && beat[0] == '.') {
                Sleep((unsigned long)(90 / BPM * 1000));
            }
            else if (beat.size() != 0 && beat[0] == '3') {
                Sleep((unsigned long)(80 / BPM * 1000));
            }
            else if (beat.size() != 0 && beat[0] == '5') {
                Sleep((unsigned long)(48 / BPM * 1000));
            }
            // 针对后有倚音
            else if (beat.size() != 0 && beat[0] == '$') {
                Sleep((unsigned long)(52 / BPM * 1000));
            }
            // 倚音
            else if (beat.size() != 0 && beat[0] == '*') {
                Sleep((unsigned long)(8 / BPM * 1000));
            }
            else {
                Sleep((unsigned long)(60 / BPM * 1000));
            }

        }
    }
};



int main()
{

    Tab tab("44",140);
    tab.Load();
    
    
   
    Sleep(6000);
    tab.Play();

    

    return 0;
    
}