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
		PlayRender(string, int, bool, bool);
		~PlayRender();
		void Rend();
		int getResult(int);

	private:
		void initialize();
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

		SpriteDrawing notes;
		SpriteDrawing userInterfaceOne;
		SpriteDrawing userInterfaceTwo;
		SpriteDrawing target;
		SpriteDrawing figures;
		SpriteDrawing judgeImage;
		SpriteDrawing pauseMenu;
		SpriteDrawing pauseCursor;
		SpriteDrawing meter;
		SpriteDrawing countdown;
		SpriteDrawing titleFrame;
		SpriteDrawing background;

		FontTextureCreate fontTexture;
		FontTextureCreate titleFrameTitle;
		
		OggPlayer *Song;

		string songName;

		DWORD currentTime;
		DWORD startTime;
		DWORD previousTime;
		DWORD pauseTime;
		DWORD countdownTime;
		DWORD framePerSecondTime;
		DWORD timing;

		vector<int> notesNumber;
		vector<int> notesMode;
		vector<int> notesDirection;
		vector<int> notesMeasure;
		vector<int> notesRhythmNumber;
		vector<int> notesDenominator;
		vector<int> notesMolecule;

		int notesDrawStart;
		int notesDrawStop;
		int statusMissCount;
		int statusBadCount;
		int statusNiceCount;
		int statusGoodCount;
		int statusGreatCount;
		int statusScore;
		int statusCurrentCombo;
		int statusMaxCombo;
		int framePerSecond;
		int framePerSecondCount;
		int sheetLevel;
		int songBpm;
		int songRhythm;
		int notesMoveSpeed;
		int dataRow;
		int notesCount;
		int pauseCursorNumber;

		bool enabled;
		bool autoPlay;
		bool customSkin;

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