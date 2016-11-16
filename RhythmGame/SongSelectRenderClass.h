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
		int selectedSheetlevel;
		string selectedSongName;

	private:
		bool searchDirectory();
		bool loadSaveData();
		void setSongNameFontPosition(int);
		
		SpriteDrawing Background;
		SpriteDrawing Title;
		SpriteDrawing Buttons;
		SpriteDrawing SongFrame;
		SpriteDrawing Back;
		SpriteDrawing Cursor;
		SpriteDrawing LevelSelect;
		SpriteDrawing LevelCursor;
		SpriteDrawing OnOff;
		SpriteDrawing Black;

		vector<FontTextureCreate> fontTexture;

		vector<string> songName;
		vector<vector<int>> sheetStars;
		vector<vector<int>> sheetLevel;
		vector<vector<int>> sheetPlayState;
		vector<vector<int>> score;

		int songCount = 1;
		int songNumber = 1;
		int scrollSongNumber = 1;
		int cursorNumber = 1;

		bool autoPlay;
		bool customSkin;

		enum class STATE {
			SONG_SELECT,
			LEVEL_SELECT,
			CONFIG,
			EXIT
		} state;
	};
}