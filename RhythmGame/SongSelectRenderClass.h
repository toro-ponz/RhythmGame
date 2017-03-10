#pragma once

#include "SpriteDrawingClass.h"
#include "FontTextureCreateClass.h"
#include <windows.h>
#include <string>
#include <iostream>

using std::string;
using namespace DxSprite;
using namespace Font;

namespace Frame {
	class SongSelectRender {
	public:
		SongSelectRender();
		~SongSelectRender();
		void Rend();
		int getSelectedSheetLevel();
		string getSelectedSongName();
		bool getAutoPlay();
		bool getCustomSkin();

	private:
		bool searchDirectory();
		bool loadSaveData();
		void setSongNameFontPosition(int);
		void setLevelStars();
		
		SpriteDrawing Background;
		SpriteDrawing Title;
		SpriteDrawing Buttons;
		SpriteDrawing SongFrame;
		SpriteDrawing Back;
		SpriteDrawing Cursor;
		SpriteDrawing LevelSelect;
		SpriteDrawing LevelCursor;
		SpriteDrawing LevelStars;
		SpriteDrawing OnOff;
		SpriteDrawing Tutorial;
		SpriteDrawing Black;

		vector<FontTextureCreate> fontTexture;
		FontTextureCreate tutorialText;

		vector<string> songName;
		vector<vector<int>> sheetStars;
		vector<vector<int>> sheetLevel;
		vector<vector<int>> sheetPlayState;
		vector<vector<int>> score;

		string selectedSongName;

		int selectedSheetlevel;
		int songCount;
		int songNumber;
		int scrollSongNumber;
		int cursorNumber;

		bool defaultAutoPlay;
		bool defaultCustomSkin;
		bool autoPlay;
		bool customSkin;
		bool tutorialDialog;

		enum class STATE {
			SONG_SELECT,
			LEVEL_SELECT,
			CONFIG,
			EXIT
		} state;
	};
}