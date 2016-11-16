#include "SongSelectRenderClass.h"
#include "GlobalVariable.h"
#include <fstream>
#include <iostream>

using std::ifstream;
using std::istringstream;

namespace Frame {
	/**
	*  コンストラクタ
	*/
	SongSelectRender::SongSelectRender() :
			Background(songselectImageDirectoryPath + "background.png"),
			Title(songselectImageDirectoryPath + "title2.png"),
			Buttons(songselectImageDirectoryPath + "buttons2.png", 2, 240.0f, 276.0f),
			SongFrame(songselectImageDirectoryPath + "frame2.png", 6, 700.0f, 214.0f),
			Cursor(songselectImageDirectoryPath + "cursor2.png", 1, 64.0f),
			LevelSelect(songselectImageDirectoryPath + "level-select.png"),
			LevelCursor(songselectImageDirectoryPath + "cursor2.png", 1, 64.0f),
			OnOff(songselectImageDirectoryPath + "on-off.png", 2, 120.0f, 75.0f),
			Black(colorDirectoryPath + "black.png") {
		Title.setPosition(0, -20.0f, -20.0f);

		Buttons.setRectFromChip(0, 1);
		Buttons.setRectFromChip(1, 2);
		Buttons.setPosition(0, 900.0f, 180.0f);
		Buttons.setPosition(1, 900.0f, 454.0f);
		
		for (int i = 0; i < 6; i++) {
			SongFrame.setRectFromChip(i, 2);
		}
		SongFrame.setPosition(0, 128.0f, 130.0f);
		SongFrame.setPosition(1, 128.0f, 234.0f);
		SongFrame.setPosition(2, 128.0f, 338.0f);
		SongFrame.setPosition(3, 128.0f, 442.0f);
		SongFrame.setPosition(4, 128.0f, 546.0f);
		SongFrame.setPosition(5, 128.0f, 650.0f);

		DWORD interval = 150;
		Cursor.setAnimationInterval(0, 1, 4, &interval, 1, 4);

		LevelSelect.setCenter(0, 290.0f, 240.0f);
		LevelSelect.setPosition(0, 512.0f, 384.0f);
		LevelSelect.disableDraw(0);

		LevelCursor.setAnimationInterval(0, 1, 4, &interval, 1, 4);
		LevelCursor.disableDraw(0);

		OnOff.setPosition(0, 620.0f, 420.0f);
		OnOff.setPosition(1, 620.0f, 493.0f);

		Black.setRectFromPixel(0, 0, 768, 0, 1024);
		Black.setPosition(0, 0.0, 0.0);
		Black.setColor(0, D3DCOLOR_ARGB(128, 255, 255, 255));
		Black.disableDraw(0);

		selectedSheetlevel = 1;

		loadSaveData();
		searchDirectory();

		songCount = songName.size();
		fontTexture.reserve(songCount);
		const string fontName = "メイリオ";
		const int songNameFontSize = 50;
		const int songNameFontWeight = 1;
		for (int i = 0; i < songCount; i++) {
			FontTextureCreate buf = FontTextureCreate();
			fontTexture.push_back(buf);
			fontTexture[i].fontCreate(songName[i], 0.0f, 0.0f, songNameFontSize, songNameFontWeight, fontName);
			fontTexture[i].setColor(D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		setSongNameFontPosition(1);
	}

	/**
	*  デストラクタ
	*/
	SongSelectRender::~SongSelectRender() {

	}

	/**
	*  描画する関数.
	*/
	void SongSelectRender::Rend() {
		const float cursorPositionY[6] = { 160.0f, 265.0f, 370.0f, 475.0f, 580.0f, 685.0f };

		Sprite->Begin(NULL);
		inputDevice->getPushState();

		if (inputDevice->getPushState(InputDevice::KEY::ARROW_UP, InputDevice::KEY_STATE::STATE_PUSH)) {
			switch (state) {
			case STATE::SONG_SELECT:
				if (cursorNumber != 1 && songNumber != 1) {
					songNumber--;
					cursorNumber--;
				}
				else if (scrollSongNumber != 1 && songNumber != 1) {
					songNumber--;
					scrollSongNumber--;
				}
				break;
			case STATE::LEVEL_SELECT:
				if (selectedSheetlevel != 1)
					selectedSheetlevel--;
				break;
			case STATE::CONFIG:
				break;
			case STATE::EXIT:
				state = STATE::CONFIG;
				break;
			}
		}
		else if (inputDevice->getPushState(InputDevice::KEY::ARROW_DOWN, InputDevice::KEY_STATE::STATE_PUSH)) {
			switch (state) {
			case STATE::SONG_SELECT:
				if (songCount < 6 && songNumber != songCount) {
					songNumber++;
					cursorNumber++;
				}
				else {
					if (cursorNumber != 6 && songNumber != songCount) {
						songNumber++;
						cursorNumber++;
					}
					else if (scrollSongNumber != songCount - 5 && songNumber != songCount) {
						songNumber++;
						scrollSongNumber++;
					}
				}
				break;
			case STATE::LEVEL_SELECT:
				if (selectedSheetlevel != 5)
					selectedSheetlevel++;
				break;
			case STATE::CONFIG:
				state = STATE::EXIT;
				break;
			case STATE::EXIT:
				break;
			}
		}		
		else if (inputDevice->getPushState(InputDevice::KEY::ARROW_LEFT, InputDevice::KEY_STATE::STATE_PUSH)) {
			switch (state) {
			case STATE::CONFIG:
			case STATE::EXIT:
				state = STATE::SONG_SELECT;
				break;
			}
		}
		else if (inputDevice->getPushState(InputDevice::KEY::ARROW_RIGHT, InputDevice::KEY_STATE::STATE_PUSH)) {
			switch (state) {
			case STATE::SONG_SELECT:
				state = STATE::CONFIG;
				break;
			}
		}
		else if (inputDevice->getPushState(InputDevice::KEY::BUTTON_RIGHT, InputDevice::KEY_STATE::STATE_PUSH) ||
			inputDevice->getPushState(InputDevice::KEY::RETURN, InputDevice::KEY_STATE::STATE_PUSH)) {
			switch (state) {
			case STATE::SONG_SELECT:
				state = STATE::LEVEL_SELECT;
				break;
			case STATE::LEVEL_SELECT:
				if (selectedSheetlevel <= 3) {
					selectedSongName = songName[songNumber - 1];
					frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::PLAY_INIT);
				}
				else if (selectedSheetlevel == 4) {
					autoPlay = !autoPlay;
				}
				else {
					customSkin = !customSkin;
				}
				break;
			case STATE::CONFIG:
				frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::CONFIG_INIT);
				break;
			case STATE::EXIT:
				frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::FINISH);
				break;
			}
		}
		else if (inputDevice->getPushState(InputDevice::KEY::BUTTON_DOWN, InputDevice::KEY_STATE::STATE_PUSH)) {
			switch (state) {
			case STATE::LEVEL_SELECT:
				state = STATE::SONG_SELECT;
				break;
			}
		}
		else if (inputDevice->getPushState(InputDevice::KEY::ESCAPE, InputDevice::KEY_STATE::STATE_PUSH)) {
			switch (state) {
			case STATE::LEVEL_SELECT:
				state = STATE::SONG_SELECT;
				break;
			default:
				state = STATE::EXIT;
				break;
			}
		}

		switch (state) {
		case STATE::SONG_SELECT:
			Buttons.setPosition(0, 880.0f, 200.0f);
			Buttons.setPosition(1, 880.0f, 480.0f);
			Cursor.enableDraw(0);
			LevelSelect.disableDraw(0);
			LevelCursor.disableDraw(0);
			OnOff.disableDraw(0);
			OnOff.disableDraw(1);
			Black.disableDraw(0);
			break;
		case STATE::LEVEL_SELECT:
			LevelSelect.enableDraw(0);
			LevelCursor.enableDraw(0);
			OnOff.enableDraw(0);
			OnOff.enableDraw(1);
			Black.enableDraw(0);
			break;
		case STATE::CONFIG:
			Buttons.setPosition(0, 880.0f - 50.0f, 200.0f);
			Buttons.setPosition(1, 880.0f, 480.0f);
			Cursor.disableDraw(0);
			break;
		case STATE::EXIT:
			Buttons.setPosition(0, 880.0f, 200.0f);
			Buttons.setPosition(1, 880.0f - 50.0f, 480.0f);
			Cursor.disableDraw(0);
			break;
		}

		setSongNameFontPosition(scrollSongNumber);
		LevelCursor.setPosition(0, 272.0f, 201.0f + ((selectedSheetlevel - 1) * 75.0f));
		Cursor.setPosition(0, 30.0f, cursorPositionY[cursorNumber - 1]);
		if (autoPlay)
			OnOff.setRectFromChip(0, 1);
		else
			OnOff.setRectFromChip(0, 2);

		if (customSkin)
			OnOff.setRectFromChip(1, 1);
		else
			OnOff.setRectFromChip(1, 2);


		Background.Draw();
		Title.Draw();
		Buttons.Draw();
		SongFrame.Draw();
		Cursor.Draw();
		for (int i = 0; i < songCount; i++) {
			fontTexture[i].Rend();
		}
		Black.Draw();
		LevelSelect.Draw();
		LevelCursor.Draw();
		OnOff.Draw();

		Sprite->End();
	}

	/**
	*  選択した譜面レベルを返す関数.
	*  @return <int> 譜面レベル(1, 2, 3)
	*/
	int SongSelectRender::getSelectedSheetLevel() {
		return selectedSheetlevel;
	}

	/**
	*  選択した曲名(ファイル名)を返す関数.
	*  @return <string> 曲名(ファイル名)
	*/
	string SongSelectRender::getSelectedSongName() {
		return selectedSongName;
	}

	/**
	*  譜面フォルダを検索する関数.
	*  @return <bool> フォルダが見つかったか否か
	*/
	bool SongSelectRender::searchDirectory() {
		WIN32_FIND_DATA findDirData;
		TCHAR *dirSearchPath = convertStringToTchar(songsDirectoryPath + "*");
		HANDLE hFind = FindFirstFile(dirSearchPath, &findDirData);
		do {
			if (hFind != INVALID_HANDLE_VALUE) {
				if (_tcscmp(findDirData.cFileName, _T(".")) && _tcscmp(findDirData.cFileName, _T(".."))) {
					if (findDirData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
						songName.push_back(convertTcharToString(findDirData.cFileName));
					}
				}
			}
		} while (FindNextFile(hFind, &findDirData));
		return true;
	}

	/**
	*  セーブデータを読み込む関数.
	*  @return <bool> セーブデータファイルが存在したか否か
	*/
	bool SongSelectRender::loadSaveData() {
		ifstream ifs(savedataFilePath);
		string str, tmp;
		getline(ifs, str);

		return true;
	}

	/**
	*  曲名の表示位置を計算・登録する関数.
	*  @param suffix<int> 一番上の曲名番号
	*/
	void SongSelectRender::setSongNameFontPosition(int suffix) {
		const float songNamePositionX[6] = { 180.0f, 180.0f, 180.0f, 180.0f, 180.0f, 180.0f };
		const float songNamePositionY[6] = { 160.0f, 265.0f, 370.0f, 475.0f, 580.0f, 685.0f };
		for (int i = 0; i < songCount; i++) {
			if (i >= suffix - 1 && i <= suffix + 4) {
				fontTexture[i].setPosition(180.0f, songNamePositionY[i - suffix + 1]);
				fontTexture[i].enableDraw();
			}
			else {
				fontTexture[i].disableDraw();
			}
		}
	}
}