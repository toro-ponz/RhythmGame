#include "PlayRenderClass.h"
#include "GlobalVariable.h"
#include <time.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <iomanip>

using std::ifstream;
using std::istringstream;
using std::ostringstream;
using std::to_string;
using std::getline;
using std::setfill;
using std::setw;

namespace Frame {
	/**
	*  コンストラクタ
	*/
	PlayRender::PlayRender(string name, int level) :
			notes(playImageDirectoryPath + "notes.png", 1, 64.0f),
			userInterfaceOne(playImageDirectoryPath + "ui1.png"),
			userInterfaceTwo(playImageDirectoryPath + "ui2.png"),
			target(playImageDirectoryPath + "target.png", 7, 128.0f),
			figures(playImageDirectoryPath + "figures.png", 14, 32.0f),
			judgeImage(playImageDirectoryPath + "judge.png", 1, 128.0f, 64.0f),
			pauseMenu(playImageDirectoryPath + "pause.png"),
			countdown(playImageDirectoryPath + "countdown.png", 1, 256.0f),
			meter(colorDirectoryPath + "red.png"),
			background() {
		const string songDirectory = songsDirectoryPath + name + "/";
		const string sheetPath = songDirectory + name + "_" + to_string(level) + ".dat";
		const string backgroundImagePath = songDirectory + "background.png";
		const string musicPath = songDirectory + name + ".ogg";

		ifstream ifs(sheetPath);
		string str, tmp;
		getline(ifs, str);
		istringstream stream(str);
		getline(stream, songName, ',');
		getline(stream, tmp, ',');
		sheetLevel = atoi(tmp.c_str());
		getline(stream, tmp, ',');
		songBpm = atoi(tmp.c_str());
		getline(stream, tmp, ',');
		songRhythm = atoi(tmp.c_str());
		getline(stream, tmp, ',');
		notesMoveSpeed = atoi(tmp.c_str());
		getline(stream, tmp, ',');
		dataRow = atoi(tmp.c_str());
		getline(stream, tmp, ';');
		notesCount = atoi(tmp.c_str());

		notesNumber.reserve(notesCount);
		notesMode.reserve(notesCount);
		notesDirection.reserve(notesCount);
		notesMeasure.reserve(notesCount);
		notesRhythmNumber.reserve(notesCount);
		notesDenominator.reserve(notesCount);
		notesMolecule.reserve(notesCount);

		notes.reserve(notesCount);

		for (int i = 0; i < notesCount; i++) {
			int doubleNotesDirection = 0;
			getline(ifs, str);
			istringstream stream(str);
			getline(stream, tmp, ',');
			notesNumber.push_back(atoi(tmp.c_str()));
			getline(stream, tmp, ',');
			notesMode.push_back(atoi(tmp.c_str()));
			getline(stream, tmp, ',');
			notesDirection.push_back(atoi(tmp.c_str()));
			getline(stream, tmp, ',');
			doubleNotesDirection = atoi(tmp.c_str());
			getline(stream, tmp, ',');
			notesMeasure.push_back(atoi(tmp.c_str()));
			getline(stream, tmp, ',');
			notesRhythmNumber.push_back(atoi(tmp.c_str()));
			getline(stream, tmp, ',');
			notesDenominator.push_back(atoi(tmp.c_str()));
			getline(stream, tmp, ';');
			notesMolecule.push_back(atoi(tmp.c_str()));
			notes.setRectFromChip(i, notesDirection[i]);
			notes.setCenter(i, 32.0f, 32.0f);
			notes.disableDraw(i);
			if (doubleNotesDirection) {
				notesNumber.push_back(notesNumber[i]);
				notesMode.push_back(notesMode[i]);
				notesDirection.push_back(doubleNotesDirection);
				notesMeasure.push_back(notesMeasure[i]);
				notesRhythmNumber.push_back(notesRhythmNumber[i]);
				notesDenominator.push_back(notesDenominator[i]);
				notesMolecule.push_back(notesMolecule[i]);
				notes.setRectFromChip(i, notesDirection[i] + 8);
				i++;
				notes.setRectFromChip(i, notesDirection[i] + 8);
				notes.setCenter(i, 32.0f, 32.0f);
				notes.disableDraw(i);
			}
		}

		target.setRectFromChip(0, 1);
		target.setRectFromChip(1, 1);
		target.setRectFromChip(2, 1);
		target.setRectFromChip(3, 1);
		target.setRectFromChip(4, 1);
		target.setRectFromChip(5, 1);
		target.setRectFromChip(6, 2);
		target.setPosition(0, 512.0f, 352.0f);
		target.setPosition(1, 512.0f, 416.0f);
		target.setPosition(2, 480.0f, 384.0f);
		target.setPosition(3, 544.0f, 384.0f);
		target.setPosition(4, 480.0f, 352.0f);
		target.setPosition(5, 544.0f, 352.0f);
		target.setPosition(6, 512.0f, 384.0f);
		target.setCenter(0, 64.0f, 64.0f);
		target.setCenter(1, 64.0f, 64.0f);
		target.setCenter(2, 64.0f, 64.0f);
		target.setCenter(3, 64.0f, 64.0f);
		target.setCenter(4, 64.0f, 64.0f);
		target.setCenter(5, 64.0f, 64.0f);
		target.setCenter(6, 64.0f, 64.0f);
		target.disableDraw(0);
		target.disableDraw(1);
		target.disableDraw(2);
		target.disableDraw(3);
		target.disableDraw(4);
		target.disableDraw(5);

		figures.setRectFromChip(4, 13, 16);
		figures.setRectFromChip(13, 29, 32);
		figures.setPosition(0, 790.0f, 0.0f);
		figures.setPosition(1, 790.0f + 25.0f, 0.0f);
		figures.setPosition(2, 790.0f + 50.0f, 0.0f);
		figures.setPosition(3, 790.0f + 75.0f, 0.0f);
		figures.setPosition(4, 767.0f, 32.0f);
		figures.setPosition(5, 690.0f, 736.0f);
		figures.setPosition(6, 690.0f + 25.0f, 736.0f);
		figures.setPosition(7, 690.0f + 50.0f, 736.0f);
		figures.setPosition(8, 690.0f + 75.0f, 736.0f);
		figures.setPosition(9, 690.0f + 100.0f, 736.0f);
		figures.setPosition(10, 690.0f + 125.0f, 736.0f);
		figures.setPosition(11, 690.0f + 150.0f, 736.0f);
		figures.setPosition(12, 690.0f + 175.0f, 736.0f);
		figures.setPosition(13, 767.0f, 704.0f);

		judgeImage.setPosition(0, 750.0f, 400.0f);
		judgeImage.disableDraw(0);

		pauseMenu.setPosition(0, 512.0f, 384.0f);
		pauseMenu.setCenter(0, 243.5f, 48.0f);
		pauseMenu.disableDraw(0);

		meter.setCenter(0, 128.0f, 768.0f);
		meter.setPosition(0, 1024.0f, 768.0f);
		meter.enableDraw(0);

		countdown.setCenter(0, 128.0f, 128.0f);
		countdown.setPosition(0, 512.0f, 384.0f);
		countdown.disableDraw(0);

		background.setTexture(backgroundImagePath);
		background.setPosition(0, 128.0f, 0.0f);
		background.enableDraw(0);

		const string fontName = "メイリオ";
		const int songNameFontSize = 60;
		const int songNameFontWeight = 1;
		fontTexture.fontCreate(songName, 50.0f, 30.0f, songNameFontSize, songNameFontWeight, fontName, false);
		fontTexture.setColor(D3DCOLOR_ARGB(255, 135, 222, 255));

		TCHAR buf[256];
		GetPrivateProfileString(_T("play"), _T("autoplay"), _T("ON"), buf, sizeof(buf), convertStringToTchar(configFilePath));
		if (!_tcscmp(buf, _T("ON"))) autoPlay = true;
		else autoPlay = false;

		Song = new OggPlayer(musicPath.c_str());
		startTime = timeGetTime();

		state = STATE_WAIT;
	}

	/**
	*  デストラクタ
	*/
	PlayRender::~PlayRender() {
		delete Song;
	}

	/**
	*  描画する関数.
	*/
	void PlayRender::Rend() {
		currentTime = timeGetTime();
		Sprite->Begin(NULL);
		inputDevice->getPushState();

		switch (state) {
		case STATE_WAIT:
			wait();
			break;
		case STATE_PLAY:
			play();
			break;
		case STATE_PAUSE:
			pause();
			break;
		case STATE_RESTART:
			restart();
			break;
		default:
			break;
		}

		Sprite->End();
		previousTime = currentTime;
	}

	/**
	*  プレイ結果を返す関数.
	*  @param mode<int> 
	*  @return<int> 結果
	*/
	int PlayRender::getResult(int mode) {
		int result;
		switch (mode) {
		case 0:
			result = statusMissCount;
			break;
		case 1:
			result = statusBadCount;
			break;
		case 2:
			result = statusNiceCount;
			break;
		case 3:
			result = statusGoodCount;
			break;
		case 4:
			result = statusGreatCount;
			break;
		case 5:
			result = statusMaxCombo;
			break;
		case 6:
			result = statusScore;
			break;
		default:
			result = 0;
			break;
		}
		return result;
	}

	/**
	*  演奏が始まるまで待つ関数.
	*/
	void PlayRender::wait() {
		calcFPS();
		updateTitle();
		draw();
		//notesMoveSpeedミリ秒待ってから再生開始
		if (Song->getState() != Dix::PCMPlayer::STATE_PLAY && ((int)(currentTime - startTime + pauseTime) >= notesMoveSpeed)) {
			Song->Play(false);
			startTime = timeGetTime() - notesMoveSpeed;
			state = STATE_PLAY;
			play();
		}
	}

	/**
	*  演奏中の関数.
	*/
	void PlayRender::play() {
		while (true) {
			if (notesDrawStop != notesCount) {
				DWORD songTime = (DWORD)((notesMeasure[notesDrawStop] - 1) * (60 * 1000) / songBpm * 4);
				songTime += (DWORD)((notesRhythmNumber[notesDrawStop] - 1) * (60 * 1000 / songBpm));
				if (notesMolecule[notesDrawStop] && notesDenominator[notesDrawStop])
					songTime += ((60 * 1000 / songBpm) * notesMolecule[notesDrawStop] / notesDenominator[notesDrawStop]);

				if (songTime <= (currentTime - startTime - pauseTime)) {
					float plus = (float)((currentTime - startTime - pauseTime - songTime) * 384.0 / notesMoveSpeed);
					notes.enableDraw(notesDrawStop);
					switch (notesDirection[notesDrawStop]) {
					case DIRECTION_FROM_TOP:
						notes.setPosition(notesDrawStop, 512.5f, -32.0f + plus);
						break;
					case DIRECTION_FROM_BOTTOM:
						notes.setPosition(notesDrawStop, 512.5f, (768.0f + 32.0f) - plus);
						break;
					case DIRECTION_FROM_LEFT:
						notes.setPosition(notesDrawStop, (128.0f - 32.0f) + plus, 384.5f);
						break;
					case DIRECTION_FROM_RIGHT:
						notes.setPosition(notesDrawStop, (896.0f + 32.0f) - plus, 384.5f);
						break;
					case DIRECTION_FROM_TOP_LEFT:
						notes.setPosition(notesDrawStop, 512.5f - (float)(384.5f / sqrt(2)) - 32.0f + plus, 384.5f - (float)(384.5f / sqrt(2)) - 32.0f + plus);
						break;
					case DIRECTION_FROM_TOP_RIGHT:
						notes.setPosition(notesDrawStop, 512.5f + (float)(384.5f / sqrt(2)) + 32.0f - plus, 384.5f - (float)(384.5f / sqrt(2)) - 32.0f + plus);
						break;
					}
					notesDrawStop++;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
		input();
		if (notesDrawStart != notesDrawStop) judge();
		calcFPS();
		updateTitle();
		draw();

		previousTime = currentTime;
		//曲が終了したらリザルト画面のフレームへ
		if (state == STATE_PLAY && Song->getState() != Dix::PCMPlayer::STATE_PLAY && Song->getState() != Dix::PCMPlayer::STATE_PAUSE) {
			frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::RESULT_INIT);
		}
	}

	/**
	*  一時停止中の関数.
	*/
	void PlayRender::pause() {
		//再生を止める
		if (Song->getState() == Dix::PCMPlayer::STATE_PLAY) {
			Song->Pause();
			countdownTime = timeGetTime();
		}
		calcFPS();
		updateTitle();
		draw();

		//STARTSELECT同時押しで再開カウントダウンへ
		if ((inputDevice->getPushState(InputDevice::KEY::BUTTON_SELECT, InputDevice::KEY_STATE::STATE_HAVE_PUSHED) &&
			inputDevice->getPushState(InputDevice::KEY::BUTTON_START, InputDevice::KEY_STATE::STATE_PUSH)) ||
			(inputDevice->getPushState(InputDevice::KEY::BUTTON_SELECT, InputDevice::KEY_STATE::STATE_PUSH) &&
			inputDevice->getPushState(InputDevice::KEY::BUTTON_START, InputDevice::KEY_STATE::STATE_HAVE_PUSHED))) {
			state = STATE_RESTART;
			pauseMenu.disableDraw(0);
			countdown.enableDraw(0);
			pauseTime += currentTime - countdownTime;
			countdownTime = timeGetTime();
		}

		//LR同時押しでSONGSELECTへ
		if ((inputDevice->getPushState(InputDevice::KEY::TRIGGER_LEFT, InputDevice::KEY_STATE::STATE_HAVE_PUSHED) &&
			inputDevice->getPushState(InputDevice::KEY::TRIGGER_RIGHT, InputDevice::KEY_STATE::STATE_PUSH)) ||
			(inputDevice->getPushState(InputDevice::KEY::TRIGGER_LEFT, InputDevice::KEY_STATE::STATE_PUSH) &&
			inputDevice->getPushState(InputDevice::KEY::TRIGGER_RIGHT, InputDevice::KEY_STATE::STATE_HAVE_PUSHED))) {
			frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::SONG_SELECT_INIT);
		}
	}

	/**
	*  一時停止から再開中の関数.
	*/
	void PlayRender::restart() {
		//5秒待って再開
		if (currentTime > countdownTime + 5000) {
			state = STATE_PLAY;
			countdown.disableDraw(0);
			Song->Play(false);
			pauseTime += currentTime - countdownTime;
		}
		else if (currentTime > countdownTime + 4000)
			countdown.setRectFromChip(0, 1);
		else if (currentTime > countdownTime + 3000)
			countdown.setRectFromChip(0, 2);
		else if (currentTime > countdownTime + 2000)
			countdown.setRectFromChip(0, 3);
		else if (currentTime > countdownTime + 1000)
			countdown.setRectFromChip(0, 4);
		else
			countdown.setRectFromChip(0, 5);

		calcFPS();
		updateTitle();
		draw();

		//LR同時押しでPause画面に戻る
		if ((inputDevice->getPushState(InputDevice::KEY::TRIGGER_LEFT, InputDevice::KEY_STATE::STATE_HAVE_PUSHED) &&
			inputDevice->getPushState(InputDevice::KEY::TRIGGER_RIGHT, InputDevice::KEY_STATE::STATE_PUSH)) ||
			(inputDevice->getPushState(InputDevice::KEY::TRIGGER_LEFT, InputDevice::KEY_STATE::STATE_PUSH) &&
			inputDevice->getPushState(InputDevice::KEY::TRIGGER_RIGHT, InputDevice::KEY_STATE::STATE_HAVE_PUSHED))) {
			state = STATE_PAUSE;
			pauseMenu.enableDraw(0);
			countdown.disableDraw(0);
			pauseTime += currentTime - countdownTime;
			countdownTime = timeGetTime();
		}
	}

	/**
	*  入力状況を表示する関数.
	*/
	void PlayRender::input() {
		for (int i = 0; i < 6; i++) {
			target.disableDraw(i);
		}

		if (inputDevice->getPushState(InputDevice::KEY::ARROW_UP, InputDevice::KEY_STATE::STATE_PUSH) ||
			inputDevice->getPushState(InputDevice::KEY::BUTTON_UP, InputDevice::KEY_STATE::STATE_PUSH))
			target.enableDraw(0);
		if (inputDevice->getPushState(InputDevice::KEY::ARROW_DOWN, InputDevice::KEY_STATE::STATE_PUSH) ||
			inputDevice->getPushState(InputDevice::KEY::BUTTON_DOWN, InputDevice::KEY_STATE::STATE_PUSH))
			target.enableDraw(1);
		if (inputDevice->getPushState(InputDevice::KEY::ARROW_LEFT, InputDevice::KEY_STATE::STATE_PUSH) ||
			inputDevice->getPushState(InputDevice::KEY::BUTTON_LEFT, InputDevice::KEY_STATE::STATE_PUSH))
			target.enableDraw(2);
		if (inputDevice->getPushState(InputDevice::KEY::ARROW_RIGHT, InputDevice::KEY_STATE::STATE_PUSH) ||
			inputDevice->getPushState(InputDevice::KEY::BUTTON_RIGHT, InputDevice::KEY_STATE::STATE_PUSH))
			target.enableDraw(3);
		if (inputDevice->getPushState(InputDevice::KEY::TRIGGER_LEFT, InputDevice::KEY_STATE::STATE_PUSH))
			target.enableDraw(4);
		if (inputDevice->getPushState(InputDevice::KEY::TRIGGER_RIGHT, InputDevice::KEY_STATE::STATE_PUSH))
			target.enableDraw(5);

		//START又はSELECT押下でポーズ
		if (inputDevice->getPushState(InputDevice::KEY::BUTTON_SELECT, InputDevice::KEY_STATE::STATE_PUSH) ||
			inputDevice->getPushState(InputDevice::KEY::BUTTON_START, InputDevice::KEY_STATE::STATE_PUSH)) {
			state = STATE_PAUSE;
			pauseMenu.enableDraw(0);
		}
	}

	/**
	*  演奏を判定する関数.
	*/
	void PlayRender::judge() {
		enum JUDGE {
			JUDGE_MISS,
			JUDGE_BAD,
			JUDGE_NICE,
			JUDGE_GOOD,
			JUDGE_GREAT
		} judge;
		const float greatRangeRate = 0.4f;
		const float goodRangeRate = 0.6f;
		const float niceRangeRate = 0.75f;
		const float badRangeRate = 1.0f;
		const float judgeRange = (float)(32.0 * 1000 / notesMoveSpeed);
		const float greatRange = judgeRange * greatRangeRate;
		const float goodRange = judgeRange * goodRangeRate;
		const float niceRange = judgeRange * niceRangeRate;
		const float badRange = judgeRange * badRangeRate;
		const float points[6] = {352.0f, 416.0f, 480.0f, 544.0f, 352.0f, 352.0f};
		const float moveAmount = (float)((DWORD)(currentTime - previousTime) * 384.0f / notesMoveSpeed);
		const float moveAmountTrigger = (float)(moveAmount / sqrt(2));

		float point;
		float pos;
		bool pushStatus[6] = {false, false, false, false, false, false};

		string arrowKeySoundEffectPath[5] = {
			 "",
			 "",
			 soundEffectDirectoryPath + "play/bt1.wav",
			 soundEffectDirectoryPath + "play/bt1.wav",
			 soundEffectDirectoryPath + "play/bt1.wav"
		};//{MISS, BAD, NICE, GOOD, GREAT}
		string triggerKeySoundEffectPath[5] = {
			 "",
			 "",
			 soundEffectDirectoryPath + "play/bt1.wav",
			 soundEffectDirectoryPath + "play/bt1.wav",
			 soundEffectDirectoryPath + "play/bt1.wav"
		};//{MISS, BAD, NICE, GOOD, GREAT}

		if (inputDevice->getPushState(InputDevice::KEY::ARROW_UP, InputDevice::KEY_STATE::STATE_PUSH) ||
			inputDevice->getPushState(InputDevice::KEY::BUTTON_UP, InputDevice::KEY_STATE::STATE_PUSH))
			pushStatus[0] = true;
		if (inputDevice->getPushState(InputDevice::KEY::ARROW_DOWN, InputDevice::KEY_STATE::STATE_PUSH) ||
			inputDevice->getPushState(InputDevice::KEY::BUTTON_DOWN, InputDevice::KEY_STATE::STATE_PUSH))
			pushStatus[1] = true;
		if (inputDevice->getPushState(InputDevice::KEY::ARROW_LEFT, InputDevice::KEY_STATE::STATE_PUSH) ||
			inputDevice->getPushState(InputDevice::KEY::BUTTON_LEFT, InputDevice::KEY_STATE::STATE_PUSH))
			pushStatus[2] = true;
		if (inputDevice->getPushState(InputDevice::KEY::ARROW_RIGHT, InputDevice::KEY_STATE::STATE_PUSH) ||
			inputDevice->getPushState(InputDevice::KEY::BUTTON_RIGHT, InputDevice::KEY_STATE::STATE_PUSH))
			pushStatus[3] = true;
		if (inputDevice->getPushState(InputDevice::KEY::TRIGGER_LEFT, InputDevice::KEY_STATE::STATE_PUSH))
			pushStatus[4] = true;
		if (inputDevice->getPushState(InputDevice::KEY::TRIGGER_RIGHT, InputDevice::KEY_STATE::STATE_PUSH))
			pushStatus[5] = true;

		vector<vector<int>> judgeNotesNumberQueue = vector<vector<int>>(6);

		for (int i = notesDrawStart; i < notesDrawStop; i++) {
			point = points[notesDirection[i] - 1];
			if (autoPlay) {
				switch (notesDirection[i]) {
				case DIRECTION_FROM_TOP:
					pos = notes.addPositionY(i, moveAmount);
					if (pos >= point)
						judgeNotesNumberQueue[0].push_back(i);
					break;
				case DIRECTION_FROM_BOTTOM:
					pos = notes.addPositionY(i, -moveAmount);
					if (pos <= point)
						judgeNotesNumberQueue[1].push_back(i);
					break;
				case DIRECTION_FROM_LEFT:
					pos = notes.addPositionX(i, moveAmount);
					if (pos >= point)
						judgeNotesNumberQueue[2].push_back(i);
					break;
				case DIRECTION_FROM_RIGHT:
					pos = notes.addPositionX(i, -moveAmount);
					if (pos <= point)
						judgeNotesNumberQueue[3].push_back(i);
					break;
				case DIRECTION_FROM_TOP_LEFT:
					notes.addPositionX(i, moveAmountTrigger);
					pos = notes.addPositionY(i, moveAmountTrigger);
					if (pos >= point)
						judgeNotesNumberQueue[4].push_back(i);
					break;
				case DIRECTION_FROM_TOP_RIGHT:
					notes.addPositionX(i, -moveAmountTrigger);
					pos = notes.addPositionY(i, moveAmountTrigger);
					if (pos >= point)
						judgeNotesNumberQueue[5].push_back(i);
					break;
				}
			}
			else {
				switch (notesDirection[i]) {
				case DIRECTION_FROM_TOP:
					pos = notes.addPositionY(i, moveAmount);
					if (pos >= point - judgeRange && pushStatus[0])
						judgeNotesNumberQueue[0].push_back(i);
					else if (pos > point + judgeRange)
						judgeNotesNumberQueue[0].push_back(i);
					break;
				case DIRECTION_FROM_BOTTOM:
					pos = notes.addPositionY(i, -moveAmount);
					if (pos <= point + judgeRange && pushStatus[1])
						judgeNotesNumberQueue[1].push_back(i);
					else if (pos < point - judgeRange)
						judgeNotesNumberQueue[1].push_back(i);
					break;
				case DIRECTION_FROM_LEFT:
					pos = notes.addPositionX(i, moveAmount);
					if (pos >= point - judgeRange && pushStatus[2])
						judgeNotesNumberQueue[2].push_back(i);
					else if (pos > point + judgeRange)
						judgeNotesNumberQueue[2].push_back(i);
					break;
				case DIRECTION_FROM_RIGHT:
					pos = notes.addPositionX(i, -moveAmount);
					if (pos <= point + judgeRange && pushStatus[3])
						judgeNotesNumberQueue[3].push_back(i);
					else if (pos < point - judgeRange)
						judgeNotesNumberQueue[3].push_back(i);
					break;
				case DIRECTION_FROM_TOP_LEFT:
					notes.addPositionX(i, moveAmountTrigger);
					pos = notes.addPositionY(i, moveAmountTrigger);
					if (pos >= point - judgeRange && pushStatus[4])
						judgeNotesNumberQueue[4].push_back(i);
					else if (pos > point + judgeRange)
						judgeNotesNumberQueue[4].push_back(i);
					break;
				case DIRECTION_FROM_TOP_RIGHT:
					notes.addPositionX(i, -moveAmountTrigger);
					pos = notes.addPositionY(i, moveAmountTrigger);
					if (pos >= point - judgeRange && pushStatus[5])
						judgeNotesNumberQueue[5].push_back(i);
					else if (pos > point + judgeRange)
						judgeNotesNumberQueue[5].push_back(i);
					break;
				}
			}
		}

		for (int i = 0; i < 6; i++) {
			int judgeNotesNumber = 0;
			float minPosition = judgeRange + 999.0f;
			for (int j = 0; j < (int)judgeNotesNumberQueue[i].size(); j++) {
				point = points[i];
				switch (i + 1) {
				case DIRECTION_FROM_TOP:
				case DIRECTION_FROM_BOTTOM:
				case DIRECTION_FROM_TOP_LEFT:
				case DIRECTION_FROM_TOP_RIGHT:
					pos = notes.addPositionY(judgeNotesNumberQueue[i][j], 0.0f);
					break;
				case DIRECTION_FROM_LEFT:
				case DIRECTION_FROM_RIGHT:
					pos = notes.addPositionX(judgeNotesNumberQueue[i][j], 0.0f);
					break;
				}

				if ((int)judgeNotesNumberQueue[i].size() == 1) {
					judgeNotesNumber = j + 1;
					break;
				}

				if ((pos > point - goodRange) && (pos < point + goodRange)) {
					judgeNotesNumber = j + 1;
					break;
				}
				else if (minPosition > fabs(point - pos)) {
					judgeNotesNumber = j + 1;
					minPosition = fabs(point - pos);
				}
			}
			if (judgeNotesNumber) {
				point = points[i];
				switch (i + 1) {
				case DIRECTION_FROM_TOP:
				case DIRECTION_FROM_BOTTOM:
				case DIRECTION_FROM_TOP_LEFT:
				case DIRECTION_FROM_TOP_RIGHT:
					pos = notes.addPositionY(judgeNotesNumberQueue[i][judgeNotesNumber - 1], 0.0f);
					break;
				case DIRECTION_FROM_LEFT:
				case DIRECTION_FROM_RIGHT:
					pos = notes.addPositionX(judgeNotesNumberQueue[i][judgeNotesNumber - 1], 0.0f);
					break;
				}

				if ((pos > point - greatRange) && (pos < point + greatRange))
					judge = JUDGE_GREAT;
				else if ((pos > point - goodRange) && (pos < point + goodRange))
					judge = JUDGE_GOOD;
				else if ((pos > point - niceRange) && (pos < point + niceRange))
					judge = JUDGE_NICE;
				else if ((pos > point - badRange) && (pos < point + badRange))
					judge = JUDGE_BAD;
				else
					judge = JUDGE_MISS;
				switch (i + 1) {
				case DIRECTION_FROM_TOP:
				case DIRECTION_FROM_BOTTOM:
				case DIRECTION_FROM_TOP_LEFT:
				case DIRECTION_FROM_TOP_RIGHT:
					playSoundEffect(arrowKeySoundEffectPath[judge]);
					break;
				case DIRECTION_FROM_LEFT:
				case DIRECTION_FROM_RIGHT:
					playSoundEffect(triggerKeySoundEffectPath[judge]);
					break;
				}
				switch (judge) {
				case JUDGE_MISS:
					notes.disableDraw(notesDrawStart);
					judgeImage.disableDraw(0);
					notesDrawStart++;
					statusMissCount++;
					statusCurrentCombo = 0;
					break;
				case JUDGE_BAD:
					notes.disableDraw(notesDrawStart);
					judgeImage.enableDraw(0);
					judgeImage.setRectFromChip(0, 1);
					notesDrawStart++;
					statusBadCount++;
					statusCurrentCombo = 0;
					statusScore += 50;
					break;
				case JUDGE_NICE:
					notes.disableDraw(notesDrawStart);
					judgeImage.enableDraw(0);
					judgeImage.setRectFromChip(0, 2);
					notesDrawStart++;
					statusNiceCount++;
					statusCurrentCombo = 0;
					statusScore += 250;
					break;
				case JUDGE_GOOD:
					notes.disableDraw(notesDrawStart);
					judgeImage.enableDraw(0);
					judgeImage.setRectFromChip(0, 3);
					notesDrawStart++;
					statusGoodCount++;
					statusCurrentCombo++;
					break;
				case JUDGE_GREAT:
					notes.disableDraw(notesDrawStart);
					judgeImage.enableDraw(0);
					judgeImage.setRectFromChip(0, 4);
					notesDrawStart++;
					statusGreatCount++;
					statusCurrentCombo++;
					break;
				}
				if (judge > JUDGE::JUDGE_NICE) {
					calcScore(judge - JUDGE::JUDGE_NICE);
				}
			}
		}
		meter.setRectFromPixel(0, 0, (LONG)(((statusGreatCount + (statusGoodCount * 0.7) + (statusNiceCount * 0.4) - (statusBadCount * 0.5) - statusMissCount) / (notesCount * 0.85)) * 768), 0, 128);
		meter.setCenter(0, 128.0, (float)(((statusGreatCount + (statusGoodCount * 0.7) + (statusNiceCount * 0.4) - (statusBadCount * 0.5) - statusMissCount) / (notesCount * 0.85)) * 768));
	}

	/**
	*  インスタンスの描画関数.
	*/
	void PlayRender::draw() {
		background.Draw();
		meter.Draw();
		userInterfaceOne.Draw();
		notes.Draw();
		target.Draw();
		userInterfaceTwo.Draw();
		fontTexture.Rend();
		judgeImage.Draw();
		figureDraw();
		pauseMenu.Draw();
		countdown.Draw();
	}

	/**
	*  スコアを計算する関数.
	*  @param judge<int> 判定結果
	*/
	void PlayRender::calcScore(int judge) {
		float rate = 1.0f;
		if (statusCurrentCombo == 100) statusScore += 10000;
		else if (statusCurrentCombo == 200) statusScore += 20000;
		else if (statusCurrentCombo == 300) statusScore += 30000;
		else if (statusCurrentCombo == 400) statusScore += 40000;
		else if (statusCurrentCombo == 500) statusScore += 50000;
		else if (statusCurrentCombo == 600) statusScore += 60000;
		else if (statusCurrentCombo == 700) statusScore += 70000;
		else if (statusCurrentCombo == 800) statusScore += 80000;
		else if (statusCurrentCombo == 900) statusScore += 90000;
		else if (statusCurrentCombo % 100 == 0) statusScore += 100000;
		if (statusCurrentCombo >= 1000) rate = 3.0f;
		else if (statusCurrentCombo >= 900) rate = 2.75f;
		else if (statusCurrentCombo >= 800) rate = 2.5f;
		else if (statusCurrentCombo >= 700) rate = 2.25f;
		else if (statusCurrentCombo >= 600) rate = 2.0f;
		else if (statusCurrentCombo >= 500) rate = 1.75f;
		else if (statusCurrentCombo >= 400) rate = 1.5f;
		else if (statusCurrentCombo >= 300) rate = 1.35f;
		else if (statusCurrentCombo >= 200) rate = 1.2f;
		else if (statusCurrentCombo >= 100) rate = 1.1f;
		else if (statusCurrentCombo >= 50) rate = 1.05f;
		statusScore += (int)(500 * judge * rate);
		if (statusScore > 99999999) statusScore = 99999999;
		if (statusCurrentCombo > statusMaxCombo) statusMaxCombo = statusCurrentCombo;
	}

	/**
	*  フレームレートを計算する関数.
	*/
	void PlayRender::calcFPS() {
		if (framePerSecondTime <= currentTime - 1000) {
			framePerSecond = framePerSecondCount;
			framePerSecondCount = 1;
			framePerSecondTime = currentTime;
		}
		else {
			framePerSecondCount++;
		}
	}

	/**
	*  ウィンドウタイトルを更新する関数.
	*/
	void PlayRender::updateTitle() {
		TCHAR windowTitle[256] = _T("RhythmGame - ");
		TCHAR tmp[256];
		switch (state) {
		case STATE_WAIT:
			lstrcat(windowTitle, _T("Waiting……"));
			break;
		case STATE_PLAY:
			lstrcat(windowTitle, _T("Playing"));
			break;
		case STATE_PAUSE:
			lstrcat(windowTitle, _T("Pausing"));
			break;
		case STATE_RESTART:
			lstrcat(windowTitle, _T("Restarting……"));
			break;
		default:
			break;
		}
		lstrcat(windowTitle, _T(" オートプレイ:"));
		if (autoPlay)
			lstrcat(windowTitle, _T("ON"));
		else
			lstrcat(windowTitle, _T("OFF"));
		lstrcat(windowTitle, _T(" FPS:"));
		_itot_s(framePerSecond, tmp, 10);
		lstrcat(windowTitle, tmp);
		::SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)windowTitle);
	}

	/**
	*  数字を描画する関数.
	*/
	void PlayRender::figureDraw() {
		ostringstream sout;
		string fig;
		sout << std::setfill(' ') << std::setw(4) << statusCurrentCombo;
		fig = sout.str();
		if (statusCurrentCombo < 10) {
			fig = "    ";
			figures.disableDraw(4);
		}
		else {
			figures.enableDraw(4);
		}
		for (int i = 0; i < 4; i++) {
			if (fig[i] == ' ') {
				figures.setRectFromChip(i, 11);
			}
			else {
				int figure = (int)(fig[i] - '0');
				figures.setRectFromChip(i, figure + 1);
			}
		}
		sout.str("");
		sout.clear();
		sout << setfill('0') << setw(8) << statusScore;
		fig = sout.str();
		for (int i = 0; i < 8; i++) {
			int figure = (int)(fig[i] - '0');
			figures.setRectFromChip(i + 5, 17 + figure);
		}

		figures.Draw();
	}

	/**
	*  SEを再生する関数.
	*  @param path<string> ファイルパス
	*/
	void PlayRender::playSoundEffect(string path) {
		if (path != "") {
			PlaySound(convertStringToTchar(path), NULL, SND_FILENAME | SND_ASYNC);
		}
	}
}