#pragma once

#include <string>
#include <vector>
#include "SpriteDrawingClass.h"
#include "FontTextureCreateClass.h"
#include "InputDevice.h"
#include "SoundPlayer.h"

using namespace Device;
using namespace DxSprite;
using namespace Sound;
using namespace Font;

using std::string;
using std::vector;

namespace Frame {
	class PlayRender {
	public:
		PlayRender(string, int);
		~PlayRender();
		void Rend();
		int getResult(int);

	private:
		void wait();
		void play();
		void pause();
		void restart();
		void input();
		void judge();
		void draw();
		void calcScore(int);
		void calcFPS();
		void updateTitle();
		void figureDraw();
		void playSoundEffect(string);

		SpriteDrawing notes;
		SpriteDrawing userInterfaceOne;
		SpriteDrawing userInterfaceTwo;
		SpriteDrawing target;
		SpriteDrawing figures;
		SpriteDrawing judgeImage;
		SpriteDrawing pauseMenu;
		SpriteDrawing meter;
		SpriteDrawing countdown;
		SpriteDrawing background;

		FontTextureCreate fontTexture;
		
		OggPlayer *Song;

		string songName;

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
		int sheetLevel;
		int songBpm;
		int songRhythm;
		int notesMoveSpeed;
		int dataRow = 0;
		int notesCount = 0;

		bool autoPlay;

		enum STATE {
			STATE_NONE,
			STATE_WAIT,
			STATE_PLAY,
			STATE_PAUSE,
			STATE_RESTART,
			STATE_FINISH
		} state;
		enum NOTES_DIRECTION {
			DIRECTION_FROM_TOP = 1,
			DIRECTION_FROM_BOTTOM,
			DIRECTION_FROM_LEFT,
			DIRECTION_FROM_RIGHT,
			DIRECTION_FROM_TOP_LEFT,
			DIRECTION_FROM_TOP_RIGHT
		};
	};
}