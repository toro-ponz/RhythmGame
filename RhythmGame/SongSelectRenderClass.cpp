#pragma comment(lib, "ShLwApi.Lib")

#include "SongSelectRenderClass.h"
#include "Global.h"
#include <shlwapi.h>
#include <fstream>
#include <sstream>
#include <iostream>

using std::ifstream;
using std::istringstream;

namespace Frame {
	/**
	*  コンストラクタ
	*/
	SongSelectRender::SongSelectRender() :
			Background(imageDirectoryPath + "background.png"),
			Title(songselectImageDirectoryPath + "title.png"),
			Buttons(songselectImageDirectoryPath + "buttons.png", 2, 240.0f, 296.0f),
			SongFrame(songselectImageDirectoryPath + "frame.png", 6, 700.0f, 214.0f),
			Cursor(songselectImageDirectoryPath + "cursor.png", 1, 64.0f),
			LevelSelect(songselectImageDirectoryPath + "level-select.png"),
			LevelCursor(songselectImageDirectoryPath + "cursor.png", 1, 64.0f),
			LevelStars(songselectImageDirectoryPath + "level-star.png", 30),
			OnOff(songselectImageDirectoryPath + "on-off.png", 2, 120.0f, 75.0f),
			Tutorial(songselectImageDirectoryPath + "tutorial.png"),
			Black(colorDirectoryPath + "black.png") {
		Title.setPosition(0, -20.0f, -20.0f);

		Buttons.setRectFromChip(0, 1);
		Buttons.setRectFromChip(1, 2);
		
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

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 10; j++) {
				LevelStars.setPosition(i * 10 + j, 550.0f + (18.0f * j), 220.0f + (76.0f * i));
			}
		}

		OnOff.setPosition(0, 620.0f, 420.0f);
		OnOff.setPosition(1, 620.0f, 493.0f);
		
		tutorialText.fontCreate(
			"●初回起動\nデフォルトのキー割り当ては以下の通りです。\n"
			"キー割り当てを変更する場合、ジョイスティック（ゲームパッド）を使用する場合は右のＣＯＮＦから設定を行ってください。\n"
			"\n"
			"[通常時]\n"
			"・カーソル移動　　：　　方向キー, WASD\n"
			"・決定　　　　　　：　　L, ENTER\n"
			"・キャンセル　　　：　　K, ESC\n"
			"・セレクト　　　　：　　F1\n"
			"・スタート　　　　：　　F2\n"
			"\n"
			"[演奏時]\n"
			"・左手　　　　　　：　　WASDQ\n"
			"・右手　　　　　　：　　IJKLP\n"
			"・ポーズ　　　　　：　　ESC, F1, F2",
			150.0f,
			120.0f,
			32,
			1,
			fontName
		);
		tutorialText.setWidthOrHeight(730.0f);

		Tutorial.setCenter(0, 425.0f, 325.0f);
		Tutorial.setPosition(0, 512.0f, 384.0f);
		if (PathFileExists(convertStringToTchar(savedataFilePath))) {
			Tutorial.disableDraw(0);
			tutorialText.disableDraw();
			tutorialDialog = false;
		}
		else {
			std::ofstream savedataFileSout;
			savedataFileSout.open(savedataFilePath, std::ios::out);
			savedataFileSout << "#savedata.dat" << std::endl;
			tutorialDialog = true;
		}

		if (!PathFileExists(convertStringToTchar(configFilePath))) {
			std::ofstream configFileSout;
			configFileSout.open(configFilePath, std::ios::out);
			string configDataInitial[40] = {
				";conf.ini",
				"",
				"[window]",
				"mode=WINDOW",
				"",
				"[directx]",
				"v-sync=ON",
				"",
				"[play]",
				"autoplay=OFF",
				"custom-skin=ON",
				"notes-timing=0",
				"",
				"[keyboard]",
				"arrow-up=17",
				"arrow-down=31",
				"arrow-left=30",
				"arrow-right=32",
				"button-up=23",
				"button-down=37",
				"button-left=36",
				"button-right=38",
				"button-select=59",
				"button-start=60",
				"trigger-left=16",
				"trigger-right=24",
				"",
				"[joystick]",
				"arrow-up=1",
				"arrow-down=2",
				"arrow-left=3",
				"arrow-right=4",
				"button-up=5",
				"button-down=6",
				"button-left=7",
				"button-right=8",
				"button-select=11",
				"button-start=12",
				"trigger-left=9",
				"trigger-right=10"
			};
			for (int i = 0; i < 40; i++) {
				configFileSout << configDataInitial[i] << std::endl;
			}
		}

		Black.setRectFromPixel(0, 0, 768, 0, 1024);
		Black.setPosition(0, 0.0, 0.0);
		Black.setColor(0, D3DCOLOR_ARGB(128, 255, 255, 255));
		Black.disableDraw(0);

		selectedSheetlevel = 1;
		songCount = 1;
		songNumber = 1;
		scrollSongNumber = 1;
		cursorNumber = 1;

		loadSaveData();
		searchDirectory();
		
		TCHAR buf[256];
		GetPrivateProfileString(_T("play"), _T("autoplay"), _T("OFF"), buf, sizeof(buf), convertStringToTchar(configFilePath));
		if (!_tcscmp(buf, _T("ON"))) {
			defaultAutoPlay = true;
			autoPlay = true;
		}
		else {
			defaultAutoPlay = false;
			autoPlay = false;
		}
		GetPrivateProfileString(_T("play"), _T("custom-skin"), _T("ON"), buf, sizeof(buf), convertStringToTchar(configFilePath));
		if (!_tcscmp(buf, _T("ON"))) {
			defaultCustomSkin = true;
			customSkin = true;
		}
		else {
			defaultCustomSkin = false;
			customSkin = false;
		}

		songCount = songName.size();
		fontTexture.reserve(songCount);
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

		if (tutorialDialog) {
			Cursor.disableDraw(0);
			if (inputDevice->getPushStateAny(InputDevice::KEY_STATE::STATE_PUSH)) {
				Tutorial.disableDraw(0);
				tutorialText.disableDraw();
				Cursor.enableDraw(0);
				tutorialDialog = false;
			}
		}
		else {
			if (inputDevice->getPushState(InputDevice::KEY::ARROW_UP, InputDevice::KEY_STATE::STATE_PUSH)) {
				switch (state) {
				case STATE::SONG_SELECT:
					if (cursorNumber != 1 && songNumber != 1) {
						playSoundEffectFromResource(4);
						songNumber--;
						cursorNumber--;
					}
					else if (scrollSongNumber != 1 && songNumber != 1) {
						playSoundEffectFromResource(4);
						songNumber--;
						scrollSongNumber--;
					}
					break;
				case STATE::LEVEL_SELECT:
					if (selectedSheetlevel != 1) {
						playSoundEffectFromResource(4);
						selectedSheetlevel--;
					}
					break;
				case STATE::EXIT:
					playSoundEffectFromResource(4);
					state = STATE::CONFIG;
					break;
				}
			}
			else if (inputDevice->getPushState(InputDevice::KEY::ARROW_DOWN, InputDevice::KEY_STATE::STATE_PUSH)) {
				switch (state) {
				case STATE::SONG_SELECT:
					if (songCount < 6 && songNumber != songCount) {
						playSoundEffectFromResource(4);
						songNumber++;
						cursorNumber++;
					}
					else {
						if (cursorNumber != 6 && songNumber != songCount) {
							playSoundEffectFromResource(4);
							songNumber++;
							cursorNumber++;
						}
						else if (scrollSongNumber != songCount - 5 && songNumber != songCount) {
							playSoundEffectFromResource(4);
							songNumber++;
							scrollSongNumber++;
						}
					}
					break;
				case STATE::LEVEL_SELECT:
					if (selectedSheetlevel != 5) {
						playSoundEffectFromResource(4);
						selectedSheetlevel++;
					}
					break;
				case STATE::CONFIG:
					playSoundEffectFromResource(4);
					state = STATE::EXIT;
					break;
				}
			}
			else if (inputDevice->getPushState(InputDevice::KEY::ARROW_LEFT, InputDevice::KEY_STATE::STATE_PUSH)) {
				switch (state) {
				case STATE::CONFIG:
				case STATE::EXIT:
					playSoundEffectFromResource(4);
					state = STATE::SONG_SELECT;
					break;
				}
			}
			else if (inputDevice->getPushState(InputDevice::KEY::ARROW_RIGHT, InputDevice::KEY_STATE::STATE_PUSH)) {
				switch (state) {
				case STATE::SONG_SELECT:
					playSoundEffectFromResource(4);
					state = STATE::CONFIG;
					break;
				}
			}
			else if (inputDevice->getPushState(InputDevice::KEY::BUTTON_RIGHT, InputDevice::KEY_STATE::STATE_PUSH) ||
				inputDevice->getPushState(InputDevice::KEY::RETURN, InputDevice::KEY_STATE::STATE_PUSH)) {
				switch (state) {
				case STATE::SONG_SELECT:
					playSoundEffectFromResource(4);
					selectedSongName = songName[songNumber - 1];
					state = STATE::LEVEL_SELECT;
					break;
				case STATE::LEVEL_SELECT:
					if (selectedSheetlevel <= 3) {
						string path = songsDirectoryPath + selectedSongName + "/" + selectedSongName + "_" + std::to_string(selectedSheetlevel) + ".dat";
						if (PathFileExists(convertStringToTchar(path))) {
							playSoundEffectFromResource(6);
							frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::PLAY_INIT);
						}
						else {
							playSoundEffectFromResource(8);
						}
					}
					else if (selectedSheetlevel == 4) {
						playSoundEffectFromResource(5);
						autoPlay = !autoPlay;
					}
					else {
						playSoundEffectFromResource(5);
						customSkin = !customSkin;
					}
					break;
				case STATE::CONFIG:
					playSoundEffectFromResource(6);
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
					playSoundEffectFromResource(7);
					autoPlay = defaultAutoPlay;
					customSkin = defaultCustomSkin;
					state = STATE::SONG_SELECT;
					break;
				}
			}
			else if (inputDevice->getPushState(InputDevice::KEY::ESCAPE, InputDevice::KEY_STATE::STATE_PUSH)) {
				switch (state) {
				case STATE::LEVEL_SELECT:
					playSoundEffectFromResource(7);
					autoPlay = defaultAutoPlay;
					customSkin = defaultCustomSkin;
					state = STATE::SONG_SELECT;
					break;
				default:
					playSoundEffectFromResource(7);
					state = STATE::EXIT;
					break;
				}
			}
		}

		switch (state) {
		case STATE::SONG_SELECT:
			Buttons.setPosition(0, 880.0f, 190.0f);
			Buttons.setPosition(1, 880.0f, 470.0f);
			Cursor.enableDraw(0);
			LevelSelect.disableDraw(0);
			LevelCursor.disableDraw(0);
			OnOff.disableDraw(0);
			OnOff.disableDraw(1);
			Black.disableDraw(0);
			for (int i = 0; i < 45; i++) {
				LevelStars.disableDraw(i);
			}
			break;
		case STATE::LEVEL_SELECT:
			LevelSelect.enableDraw(0);
			LevelCursor.enableDraw(0);
			OnOff.enableDraw(0);
			OnOff.enableDraw(1);
			Black.enableDraw(0);
			setLevelStars();
			break;
		case STATE::CONFIG:
			Buttons.setPosition(0, 880.0f - 50.0f, 190.0f);
			Buttons.setPosition(1, 880.0f, 470.0f);
			Cursor.disableDraw(0);
			break;
		case STATE::EXIT:
			Buttons.setPosition(0, 880.0f, 190.0f);
			Buttons.setPosition(1, 880.0f - 50.0f, 470.0f);
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
		LevelStars.Draw();
		OnOff.Draw();
		Tutorial.Draw();
		tutorialText.Rend();

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
	*  オートプレイがONかどうかを返す関数.
	*  @return <bool> オートプレイのON, OFF
	*/
	bool SongSelectRender::getAutoPlay() {
		return autoPlay;
	}

	/**
	*  カスタムスキンがONかどうかを返す関数.
	*  @return <bool> カスタムスキンのON, OFF
	*/
	bool SongSelectRender::getCustomSkin() {
		return customSkin;
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

	/**
	*  難易度の星の数を計算する関数.
	*/
	void SongSelectRender::setLevelStars() {
		for (int i = 0; i < 3; i++) {
			string path = songsDirectoryPath + selectedSongName + "/" + selectedSongName + "_" + std::to_string(i + 1) + ".dat";
			if (PathFileExists(convertStringToTchar(path))) {
				ifstream ifs(path);
				string str, tmp;
				std::getline(ifs, str);
				istringstream stream(str);
				std::getline(stream, tmp, ',');
				std::getline(stream, tmp, ',');
				std::getline(stream, tmp, ',');
				std::getline(stream, tmp, ',');
				std::getline(stream, tmp, ',');
				std::getline(stream, tmp, ',');
				std::getline(stream, tmp, ',');
				std::getline(stream, tmp, ',');
				std::getline(stream, tmp, ';');
				int stars = atoi(tmp.c_str());

				for (int j = 0; j < stars; j++) {
					LevelStars.enableDraw(i * 10 + j);
				}
				for (int j = stars; j < 10; j++) {
					LevelStars.disableDraw(i * 10 + j);
				}
			}
			else {
				for (int j = 0; j < 10; j++) {
					LevelStars.disableDraw(i * 10 + j);
				}
			}
		}
	}
}