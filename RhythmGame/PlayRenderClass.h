#pragma once

#include <string>
#include <vector>
#include "SpriteDrawingClass.h"
#include "InputDevice.h"
#include "SoundPlayer.h"

#define SONG_SELECT_INIT     25
#define PAUSE_INIT           45
#define RESULT_INIT          50

using namespace Device;
using namespace DxSprite;
using namespace Sound;
using namespace std;

namespace Frame {
	class PlayRender {
	public:
		PlayRender(int);
		~PlayRender();
		void Rend(void);
		int getResult(int);

	private:
		void Play();
		void input();
		void judge();
		void calcScore(int);
		void calcFPS();
		void updateTitle();
		void figureDraw();
		void playSoundEffect(TCHAR*);
		void Pause();
		void Restart();

		SpriteDrawing Notes;
		SpriteDrawing sprite;
		SpriteDrawing Meter;
		SpriteDrawing Pau;
		SpriteDrawing Countdown;
		OggPlayer * Song;

		string songName;
		string songFileName;

		DWORD currentTime;
		DWORD startTime;
		DWORD previousTime;
		DWORD pauseTime = 0;
		DWORD countdownTime;
		DWORD framePerSecondTime;

		vector<int> notesNumber;
		vector<int> notesMode;
		vector<int> notesDirection;
		vector<int> notesMeasure;
		vector<int> notesRhythmNumber;
		vector<int> notesDenominator;
		vector<int> notesMolecule;

		int notesDrawStart = 0;
		int notesDrawStop = 0;
		int statusMissCount = 0;
		int statusBadCount = 0;
		int statusNiceCount = 0;
		int statusGoodCount = 0;
		int statusGreatCount = 0;
		int statusScore = 0;
		int statusCurrentCombo = 0;
		int statusMaxCombo = 0;
		int framePerSecond = 0;
		int framePerSecondCount = 0;
		int scoreLevel;
		int songBpm;
		int songRhythm;
		int notesMoveSpeed;
		int dataRow = 0;
		int notesCount = 0;

		bool statePause = false;
		bool stateRestart = false;
		bool statePlay = false;
		bool autoPlay;
	};
}