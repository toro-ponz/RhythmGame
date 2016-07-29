#pragma once

#include <string>
#include <vector>
#include "SpriteDrawingClass2.h"
#include "SpriteDrawingClass3.h"
#include "InputDeviceClass.h"
#include "OggPlayClass.h"
#include "WavPlayClass.h"

#define SONG_SELECT_INIT     25
#define PAUSE_INIT           45
#define RESULT_INIT          50

using namespace Device;
using namespace DxSprite;
using namespace Ogg;
using namespace Wav;
using namespace std;

namespace Frame {
	class PlayRender {
	public:
		PlayRender(int);
		~PlayRender();
		void Rend(void);
		int getResult(int);

	private:
		void Play(DWORD);
		void JoyStick();
		void judge(DWORD);
		void calcScore();
		void calcFPS(DWORD);
		void updateTitle();
		void numDraw();
		void SE(TCHAR *);
		void Pause(DWORD);
		void Restart(DWORD);

		SpriteDrawing2 * Notes;
		SpriteDrawing3 Sprite;
		SpriteDrawing2 Meter;
		SpriteDrawing2 Pau;
		SpriteDrawing2 Countdown;
		OggPlay * Song;

		DWORD sTime;
		DWORD oTime;
		DWORD pTime = 0;
		DWORD xTime;
		DWORD fpsTime;
		int start = 0;
		int stop = 0;
		int miss = 0;
		int bad = 0;
		int nice = 0;
		int good = 0;
		int great = 0;
		int fps = 0;
		int cfps = 0;
		int score = 0;
		int combo = 0;
		int maxCombo = 0;
		string songName;
		string fileName;
		int level;
		int bpm;
		int rhythm;
		int speed;
		int datacnt;

		vector<int> num;
		vector<int> mode;
		vector<int> dist1;
		vector<int> dist2;
		vector<int> nodule;
		vector<int> rhythmnum;
		vector<int> mom;
		vector<int> son;

		bool pause = false;
		bool restart = false;
		bool flag = false;
		bool auto_play;
	};
}