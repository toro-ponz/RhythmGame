#define _CRT_SECURE_NO_WARNINGS

#include "PlayRenderClass.h"
#include <time.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <iomanip>

extern BYTE FrameNumber;
extern InputDevice * inputDevice;
extern HWND hWnd;

using namespace DxSprite;

namespace Frame {
	//コンストラクタ
	PlayRender::PlayRender(int lv) :
			sprite		(_T("Data/img/play/chip01.png"), 29, 32.0f),
			Notes       (_T("Data/img/play/notes.png"), 1, 64.0f),
			Meter		(1),
			Pau			(1),
			Countdown	(1) {
		string sheetFile = "Data/songs/Sample/Sample_" + to_string(lv) + ".dat";
		ifstream ifs(sheetFile);

		string str, tmp;
		getline(ifs, str);
		istringstream stream(str);
		getline(stream, songName, ',');
		getline(stream, songFileName, ',');
		getline(stream, tmp, ',');
		scoreLevel = atoi(tmp.c_str());
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

		Notes.reserve(notesCount);

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
			Notes.setCenter(i, 32.0f, 32.0f);
			Notes.setRectC(i, notesDirection[i], notesDirection[i]);
			if (doubleNotesDirection) {
				notesNumber.push_back(notesNumber[i]);
				notesMode.push_back(notesMode[i]);
				notesDirection.push_back(doubleNotesDirection);
				notesMeasure.push_back(notesMeasure[i]);
				notesRhythmNumber.push_back(notesRhythmNumber[i]);
				notesDenominator.push_back(notesDenominator[i]);
				notesMolecule.push_back(notesMolecule[i]);
				Notes.setCenter(i, 32.0f, 32.0f);
				Notes.setRectC(i, notesDirection[i] + 8, notesDirection[i] + 8);
				i++;
				Notes.setCenter(i, 32.0f, 32.0f);
				Notes.setRectC(i, notesDirection[i] + 8, notesDirection[i] + 8);
			}
		}

		sprite.setRectC(0, 1, 740);
		sprite.setRect(1, 256, 768, 128, 0);
		sprite.setRect(2, 32, 800, 0, 768);
		sprite.setRect(3, 32, 800, 0, 768);
		sprite.setRect(4, 32, 800, 0, 768);
		sprite.setRect(5, 32, 800, 0, 768);
		sprite.setRect(6, 128, 896, 0, 864);
		sprite.setRect(7, 32, 928, 0, 896);
		sprite.setRect(8, 32, 928, 0, 896);
		sprite.setRect(9, 32, 928, 0, 896);
		sprite.setRect(10, 32, 928, 0, 896);
		sprite.setRect(11, 32, 928, 0, 896);
		sprite.setRect(12, 32, 928, 0, 896);
		sprite.setRect(13, 32, 928, 0, 896);
		sprite.setRect(14, 32, 928, 0, 896);
		sprite.setRect(15, 32, 1024, 0, 992);
		sprite.setRect(16, 64, 1024, 32, 992);
		sprite.setRect(17, 96, 1024, 64, 992);
		sprite.setRect(18, 128, 1024, 96, 992);
		sprite.setRect(19, 96, 992, 64, 960);
		sprite.setRect(20, 128, 992, 96, 960);
		sprite.setRect(21, 256, 1024, 128, 960);
		sprite.setRect(22, 1024, 768, 256, 0);
		sprite.setRect(23, 384, 896, 256, 768);
		sprite.setRect(24, 384, 896, 256, 768);
		sprite.setRect(25, 384, 896, 256, 768);
		sprite.setRect(26, 384, 896, 256, 768);
		sprite.setRect(27, 384, 896, 256, 768);
		sprite.setRect(28, 384, 896, 256, 768);
		sprite.setPosition(0, 0.0f, 0.0f);
		sprite.setPosition(1, 896.0f, 0.0f);
		for (int i = 0; i < 5; i++) {
			sprite.setPosition(i + 2, (float)(650.0 + (i * 23.0)), 580.0);
		}
		for (int i = 0; i < 8; i++) {
			sprite.setPosition(i + 7, (float)(700.0 + (i * 20.0)), (float)(768.0 - 40.0));
		}
		sprite.setPosition(15, 128.0, 700.0);
		sprite.setPosition(16, 128.0 + 32.0, 700.0);
		sprite.setPosition(17, 128.0 + 64.0, 700.0);
		sprite.setPosition(18, 128.0 + 96.0, 700.0);
		sprite.setPosition(19, 128.0 + 128.0, 700.0);
		sprite.setPosition(20, 128.0 + 160.0, 700.0);
		sprite.setPosition(21, 768.0, 512.0);
		sprite.setPosition(22, 128.0f, 0.0f);

		sprite.setPosition(23, 512.0f, 352.0f);
		sprite.setPosition(24, 512.0f, 416.0f);
		sprite.setPosition(25, 544.0f, 384.0f);
		sprite.setPosition(26, 480.0f, 384.0f);
		sprite.setPosition(27, 544.0f, 352.0f);
		sprite.setPosition(28, 480.0f, 352.0f);

		sprite.setCenter(23, 64.0f, 64.0f);
		sprite.setCenter(24, 64.0f, 64.0f);
		sprite.setCenter(25, 64.0f, 64.0f);
		sprite.setCenter(26, 64.0f, 64.0f);
		sprite.setCenter(27, 64.0f, 64.0f);
		sprite.setCenter(28, 64.0f, 64.0f);

		sprite.disableDraw(21);

		Meter.setTexture(_T("Data/img/color/red.png"));
		Meter.setCenter(0, 128.0, 768.0);
		Meter.setPosition(0, 1024.0, 768.0);
		Meter.enableDraw(0);

		Pau.setTexture(_T("Data/img/play/pause.png"));
		Pau.setCenter(0, 256.0f, 128.0f);
		Pau.setPosition(0, 512.0f, 384.0f);
		Pau.enableDraw(0);

		Countdown.setTexture(_T("Data/img/play/cntdwn.png"));
		Countdown.setCenter(0, 128.0f, 128.0f);
		Countdown.setPosition(0, 512.0f, 384.0f);
		Countdown.enableDraw(0);

		TCHAR buf[100];
		GetPrivateProfileString(_T("common"), _T("autoplay"), _T("ON"), buf, sizeof(buf), _T("Data/conf.ini"));
		if (!wcscmp(buf, _T("ON"))) autoPlay = true;
		else autoPlay = false;

		string musicFile = "Data/songs/" + songName + "/" + songFileName;
		Song = new OggPlayer(musicFile.c_str());
		startTime = timeGetTime();
	}

	//デストラクタ
	PlayRender::~PlayRender() {
		delete Song;
	}

	//Rend関数
	void PlayRender::Rend(void) {
		Sprite->Begin(NULL);
		currentTime = timeGetTime();
		inputDevice->getPushState();
		if (!statePause) {
			Play();
		}
		else {
			if (!stateRestart) Pause();
			else Restart();
		}
		previousTime = currentTime;
		Sprite->End();
	}

	//getResult関数
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

	//Play関数
	void PlayRender::Play() {
		//notesMoveSpeedミリ秒待ってから再生開始
		if (Song->getState() != Dix::PCMPlayer::STATE_PLAY && !statePlay && ((int)(currentTime - startTime + pauseTime) >= notesMoveSpeed)) {
			Song->Play(false);
			statePlay = true;
			startTime = timeGetTime() - notesMoveSpeed;
		}

		while (true) {
			if (notesDrawStop != notesCount) {
				DWORD songTime = (DWORD)((notesMeasure[notesDrawStop] - 1) * (60 * 1000) / songBpm * 4);
				songTime += (DWORD)((notesRhythmNumber[notesDrawStop] - 1) * (60 * 1000 / songBpm));
				if (notesMolecule[notesDrawStop] && notesDenominator[notesDrawStop])
					songTime += ((60 * 1000 / songBpm) * notesMolecule[notesDrawStop] / notesDenominator[notesDrawStop]);

				if (songTime <= (currentTime - startTime - pauseTime)) {
					float plus = (float)((currentTime - startTime - pauseTime - songTime) * 384.0 / notesMoveSpeed);
					Notes.enableDraw(notesDrawStop);
					switch (notesDirection[notesDrawStop]) {
					case 1:
						Notes.setPosition(notesDrawStop, 512.0f, -32.0f + plus);
						break;
					case 2:
						Notes.setPosition(notesDrawStop, 512.0f, (768.0f + 32.0f) - plus);
						break;
					case 3:
						Notes.setPosition(notesDrawStop, (896.0f + 32.0f) - plus, 384.0f);
						break;
					case 4:
						Notes.setPosition(notesDrawStop, (128.0f - 32.0f) + plus, 384.0f);
						break;
					case 5:
						Notes.setPosition(notesDrawStop, 896.0f + 32.0f - plus, -32.0f + plus);
						break;
					case 6:
						Notes.setPosition(notesDrawStop, 128.0f - 32.0f + plus, -32.0f + plus);
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
		figureDraw();
		Notes.Draw();
		Meter.Draw();
		sprite.Draw();

		previousTime = currentTime;
		//曲が終了したらリザルト画面のフレームへ
		if (statePlay && Song->getState() != Dix::PCMPlayer::STATE_PLAY && Song->getState() != Dix::PCMPlayer::STATE_PAUSE) {
			FrameNumber = RESULT_INIT;
		}
	}

	//input関数
	void PlayRender::input() {
		for (int i = 0; i < 6; i++) {
			sprite.disableDraw(15 + i);
			sprite.disableDraw(23 + i);
		}
		if (inputDevice->getPushState(0, 0) || inputDevice->getPushState(6, 0))
			sprite.enableDraw(15);
		if (inputDevice->getPushState(1, 0) || inputDevice->getPushState(5, 0))
			sprite.enableDraw(16);
		if (inputDevice->getPushState(2, 0) || inputDevice->getPushState(4, 0))
			sprite.enableDraw(17);
		if (inputDevice->getPushState(3, 0) || inputDevice->getPushState(7, 0))
			sprite.enableDraw(18);
		if (inputDevice->getPushState(8, 0))
			sprite.enableDraw(19);
		if (inputDevice->getPushState(9, 0))
			sprite.enableDraw(20);

		if (inputDevice->getPushState(0, 1) || inputDevice->getPushState(6, 1))
			sprite.enableDraw(23);
		if (inputDevice->getPushState(1, 1) || inputDevice->getPushState(5, 1))
			sprite.enableDraw(24);
		if (inputDevice->getPushState(2, 1) || inputDevice->getPushState(4, 1))
			sprite.enableDraw(25);
		if (inputDevice->getPushState(3, 1) || inputDevice->getPushState(7, 1))
			sprite.enableDraw(26);
		if (inputDevice->getPushState(9, 1))
			sprite.enableDraw(27);
		if (inputDevice->getPushState(8, 1))
			sprite.enableDraw(28);

		//START又はSELECT押下でポーズ
		if (inputDevice->getPushState(10, 1) || inputDevice->getPushState(11, 1))
			statePause = true;
	}

	//judge関数
	void PlayRender::judge() {
		int judge = 0; //1:MISS, 2:BAD, 3:NICE, 4:GOOD, 5:GREAT
		const float distx = 512.0;
		const float disty = 384.0;
		const float greatRange = 0.4f;
		const float goodRange = 0.6f;
		const float niceRange = 0.75f;
		const float badRange = 1.0f;
		const float out = (float)(32.0 * 1000 / notesMoveSpeed);
		const float p_great = out * greatRange;
		const float p_good = out * goodRange;
		const float p_nice = out * niceRange;
		const float p_bad = out * badRange;
		const float points[6] = { 352.0f, 416.0f, 544.0f, 480.0f, 352.0f, 352.0f };

		float add = (float)((DWORD)(currentTime - previousTime) * 384.0 / notesMoveSpeed);
		float point;
		float pos;
		bool pushStatus[6] = { false, false, false, false, false, false };

		TCHAR *arrowKeySoundEffectPath[5] = {
			 _T(""), _T(""), _T("Data/se/play/bt1.wav"), _T("Data/se/play/bt1.wav"), _T("Data/se/play/bt1.wav")
		};//{MISS, BAD, NICE, GOOD, GREAT}
		TCHAR *triggerKeySoundEffectPath[5] = {
			 _T(""), _T(""), _T("Data/se/play/bt1.wav"), _T("Data/se/play/bt1.wav"), _T("Data/se/play/bt1.wav")
		};//{MISS, BAD, NICE, GOOD, GREAT}

		if (inputDevice->getPushState(0, 1) || inputDevice->getPushState(6, 1))
			pushStatus[0] = true;
		if (inputDevice->getPushState(1, 1) || inputDevice->getPushState(5, 1))
			pushStatus[1] = true;
		if (inputDevice->getPushState(2, 1) || inputDevice->getPushState(4, 1))
			pushStatus[2] = true;
		if (inputDevice->getPushState(3, 1) || inputDevice->getPushState(7, 1))
			pushStatus[3] = true;
		if (inputDevice->getPushState(9, 1))
			pushStatus[4] = true;
		if (inputDevice->getPushState(8, 1))
			pushStatus[5] = true;

		vector<vector<int>> judgeNotesNumberQueue = vector<vector<int>>(6);

		for (int i = notesDrawStart; i < notesDrawStop; i++) {
			point = points[notesDirection[i] - 1];
			if (autoPlay) {
				switch (notesDirection[i]) {
				case 1:
					pos = Notes.addPositionY(add, i);
					if (pos >= point)
						judgeNotesNumberQueue[0].push_back(i);
					break;
				case 2:
					pos = Notes.addPositionY(-add, i);
					if (pos <= point)
						judgeNotesNumberQueue[1].push_back(i);
					break;
				case 3:
					pos = Notes.addPositionX(-add, i);
					if (pos <= point)
						judgeNotesNumberQueue[2].push_back(i);
					break;
				case 4:
					pos = Notes.addPositionX(add, i);
					if (pos >= point)
						judgeNotesNumberQueue[3].push_back(i);
					break;
				case 5:
					Notes.addPositionX(-add, i);
					pos = Notes.addPositionY(add, i);
					if (pos >= point)
						judgeNotesNumberQueue[4].push_back(i);
					break;
				case 6:
					Notes.addPositionX(add, i);
					pos = Notes.addPositionY(add, i);
					if (pos >= point)
						judgeNotesNumberQueue[5].push_back(i);
					break;
				}
			}
			else {
				switch (notesDirection[i]) {
				case 1:
					pos = Notes.addPositionY(add, i);
					if (pos >= point - out && pushStatus[0])
						judgeNotesNumberQueue[0].push_back(i);
					else if (pos > point + out)
						judgeNotesNumberQueue[0].push_back(i);
					break;
				case 2:
					pos = Notes.addPositionY(-add, i);
					if (pos <= point + out && pushStatus[1])
						judgeNotesNumberQueue[1].push_back(i);
					else if (pos < point - out)
						judgeNotesNumberQueue[1].push_back(i);
					break;
				case 3:
					pos = Notes.addPositionX(-add, i);
					if (pos <= point + out && pushStatus[2])
						judgeNotesNumberQueue[2].push_back(i);
					else if (pos < point - out)
						judgeNotesNumberQueue[2].push_back(i);
					break;
				case 4:
					pos = Notes.addPositionX(add, i);
					if (pos >= point - out && pushStatus[3])
						judgeNotesNumberQueue[3].push_back(i);
					else if (pos > point + out)
						judgeNotesNumberQueue[3].push_back(i);
					break;
				case 5:
					Notes.addPositionX(-add, i);
					pos = Notes.addPositionY(add, i);
					if (pos >= point - out && pushStatus[4])
						judgeNotesNumberQueue[4].push_back(i);
					else if (pos > point + out)
						judgeNotesNumberQueue[4].push_back(i);
					break;
				case 6:
					Notes.addPositionX(add, i);
					pos = Notes.addPositionY(add, i);
					if (pos >= point - out && pushStatus[5])
						judgeNotesNumberQueue[5].push_back(i);
					else if (pos > point + out)
						judgeNotesNumberQueue[5].push_back(i);
					break;
				}
			}
		}

		for (int i = 0; i < 6; i++) {
			int judgeNotesNumber[6] = {};
			float minPosition = out + 999.0f;
			for (int j = 0; j < (int)judgeNotesNumberQueue[i].size(); j++) {
				point = points[i];
				switch (i + 1) {
				case 1:
				case 2:
				case 5:
				case 6:
					pos = Notes.addPositionY(0.0f, judgeNotesNumberQueue[i][j]);
					break;
				case 3:
				case 4:
					pos = Notes.addPositionX(0.0f, judgeNotesNumberQueue[i][j]);
					break;
				}

				if ((int)judgeNotesNumberQueue[i].size() == 1) {
					judgeNotesNumber[i] = j + 1;
					break;
				}

				if ((pos > point - p_good) && (pos < point + p_good)) {
					judgeNotesNumber[i] = j + 1;
					break;
				}
				else if (minPosition > fabs(point - pos)) {
					judgeNotesNumber[i] = j + 1;
					minPosition = fabs(point - pos);
				}
			}
			if (judgeNotesNumber[i]) {
				point = points[i];
				switch (i + 1) {
				case 1:
				case 2:
				case 5:
				case 6:
					pos = Notes.addPositionY(0.0f, judgeNotesNumberQueue[i][judgeNotesNumber[i] - 1]);
					break;
				case 3:
				case 4:
					pos = Notes.addPositionX(0.0f, judgeNotesNumberQueue[i][judgeNotesNumber[i] - 1]);
					break;
				}

				if ((pos > point - p_great) && (pos < point + p_great))
					judge = 5;
				else if ((pos > point - p_good) && (pos < point + p_good))
					judge = 4;
				else if ((pos > point - p_nice) && (pos < point + p_nice))
					judge = 3;
				else if ((pos > point - p_bad) && (pos < point + p_bad))
					judge = 2;
				else
					judge = 1;
				switch (i + 1) {
				case 1:
				case 2:
				case 3:
				case 4:
					playSoundEffect(arrowKeySoundEffectPath[judge - 1]);
					break;
				case 5:
				case 6:
					playSoundEffect(triggerKeySoundEffectPath[judge -1]);
					break;
				}
				switch (judge) {
				case 1: //MISSのとき
					Notes.disableDraw(notesDrawStart);
					sprite.disableDraw(21);
					notesDrawStart++;
					statusMissCount++;
					statusCurrentCombo = 0;
					break;
				case 2: //BADのとき
					Notes.disableDraw(notesDrawStart);
					sprite.enableDraw(21);
					sprite.setRect(21, 256, 832, 128, 768);
					notesDrawStart++;
					statusBadCount++;
					statusCurrentCombo = 0;
					statusScore += 50;
					break;
				case 3: //NICEのとき
					Notes.disableDraw(notesDrawStart);
					sprite.enableDraw(21);
					sprite.setRect(21, 256, 896, 128, 832);
					notesDrawStart++;
					statusNiceCount++;
					statusCurrentCombo = 0;
					statusScore += 150;
					break;
				case 4: //GOODのとき
					Notes.disableDraw(notesDrawStart);
					sprite.enableDraw(21);
					sprite.setRect(21, 256, 960, 128, 896);
					notesDrawStart++;
					statusGoodCount++;
					statusCurrentCombo++;
					break;
				case 5: //GREATのとき
					Notes.disableDraw(notesDrawStart);
					sprite.enableDraw(21);
					sprite.setRect(21, 256, 1024, 128, 960);
					notesDrawStart++;
					statusGreatCount++;
					statusCurrentCombo++;
					break;
				}
				if (judge > 3) {
					calcScore(judge);
				}
			}
		}
		Meter.setRect(0, 128, (LONG)(((statusGreatCount + (statusGoodCount * 0.7) + (statusNiceCount * 0.4) - (statusBadCount * 0.5) - statusMissCount) / (notesCount * 0.85)) * 768));
		Meter.setCenter(0, 128.0, (float)(((statusGreatCount + (statusGoodCount * 0.7) + (statusNiceCount * 0.4) - (statusBadCount * 0.5) - statusMissCount) / (notesCount * 0.85)) * 768));
	}

	//calcScore関数
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
		statusScore += (int)(500 * (judge - 3) * rate);
		if (statusScore > 99999999) statusScore = 99999999;
		if (statusCurrentCombo > statusMaxCombo) statusMaxCombo = statusCurrentCombo;
	}

	//calcFPS関数
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

	//updateTitle関数
	void PlayRender::updateTitle() {
		TCHAR lpsz[100] = _T("RhythmGame (Miss:");
		TCHAR xxx[12] = _T("00000");
		wcscat(lpsz, _itow(statusMissCount, xxx, 10));
		wcscat(lpsz, _T(", Bad:"));
		wcscat(lpsz, _itow(statusBadCount, xxx, 10));
		wcscat(lpsz, _T(", Nice:"));
		wcscat(lpsz, _itow(statusNiceCount, xxx, 10));
		wcscat(lpsz, _T(", Good:"));
		wcscat(lpsz, _itow(statusGoodCount, xxx, 10));
		wcscat(lpsz, _T(", Great:"));
		wcscat(lpsz, _itow(statusGreatCount, xxx, 10));
		wcscat(lpsz, _T(",  / "));
		wcscat(lpsz, _itow(notesDrawStart, xxx, 10));
		wcscat(lpsz, _T(") FPS : "));
		wcscat(lpsz, _itow(framePerSecond, xxx, 10));
		::SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)lpsz);
	}

	//numDraw関数
	void PlayRender::figureDraw() {
		std::ostringstream sout;
		sout << std::setfill('0') << std::setw(8) << statusScore;
		std::string s = sout.str();
		for (int i = 0; i < 8; i++) {
			switch (s[i]) {
			case '0':
				sprite.setRect(i + 7, 32, 928, 0, 896);
				break;
			case '1':
				sprite.setRect(i + 7, 64, 928, 32, 896);
				break;
			case '2':
				sprite.setRect(i + 7, 96, 928, 64, 896);
				break;
			case '3':
				sprite.setRect(i + 7, 128, 928, 96, 896);
				break;
			case '4':
				sprite.setRect(i + 7, 32, 960, 0, 928);
				break;
			case '5':
				sprite.setRect(i + 7, 64, 960, 32, 928);
				break;
			case '6':
				sprite.setRect(i + 7, 96, 960, 64, 928);
				break;
			case '7':
				sprite.setRect(i + 7, 128, 960, 96, 928);
				break;
			case '8':
				sprite.setRect(i + 7, 32, 992, 0, 960);
				break;
			case '9':
				sprite.setRect(i + 7, 64, 992, 32, 960);
				break;
			}
		}
		std::ostringstream sout2;
		sout2 << std::setfill(' ') << std::setw(4) << statusCurrentCombo;
		s = sout2.str();
		if (statusCurrentCombo < 10) {
			s = "    ";
			sprite.disableDraw(6);
		}
		else {
			sprite.enableDraw(6);
		}
		for (int i = 0; i < 4; i++) {
			switch (s[i]) {
			case ' ':
				sprite.setRect(i + 2, 96, 864, 64, 832);
				break;
			case '0':
				sprite.setRect(i + 2, 32, 800, 0, 768);
				break;
			case '1':
				sprite.setRect(i + 2, 64, 800, 32, 768);
				break;
			case '2':
				sprite.setRect(i + 2, 96, 800, 64, 768);
				break;
			case '3':
				sprite.setRect(i + 2, 128, 800, 96, 768);
				break;
			case '4':
				sprite.setRect(i + 2, 32, 832, 0, 800);
				break;
			case '5':
				sprite.setRect(i + 2, 64, 832, 32, 800);
				break;
			case '6':
				sprite.setRect(i + 2, 96, 832, 64, 800);
				break;
			case '7':
				sprite.setRect(i + 2, 128, 832, 96, 800);
				break;
			case '8':
				sprite.setRect(i + 2, 32, 864, 0, 832);
				break;
			case '9':
				sprite.setRect(i + 2, 64, 864, 32, 832);
				break;
			}
		}
	}

	//playSoundEffect関数
	void PlayRender::playSoundEffect(TCHAR *path) {
		if (path != _T("")) {
			PlaySound(path, NULL, SND_FILENAME | SND_ASYNC);
		}
	}

	//Pause関数
	void PlayRender::Pause() {
		//再生を止める
		if (Song->getState() == Dix::PCMPlayer::STATE_PLAY) {
			Song->Pause();
			countdownTime = timeGetTime();
		}
		calcFPS();
		updateTitle();
		figureDraw();
		//Background->Draw();
		Notes.Draw();
		Meter.Draw();
		sprite.Draw();
		Pau.Draw();

		//STARTSELECT同時押しで再開カウントダウンへ
		if ((inputDevice->getPushState(10, 0) && (inputDevice->getPushState(11, 0)))) {
			stateRestart = true;
			pauseTime += currentTime - countdownTime;
			countdownTime = timeGetTime();
		}

		//LR同時押しでSONGSELECTへ
		if ((inputDevice->getPushState(8, 0) && (inputDevice->getPushState(9, 0)))) {
			FrameNumber = SONG_SELECT_INIT;
		}
	}

	//Restart関数
	void PlayRender::Restart() {
		//5秒待って再開
		if (currentTime > countdownTime + 5000) {
			statePause = false;
			stateRestart = false;
			Song->Play(false);
			pauseTime += currentTime - countdownTime;
		}
		else if (currentTime > countdownTime + 4000) {
			Countdown.setRect(0, 256, 256, 0, 0);
		}
		else if (currentTime > countdownTime + 3000) {
			Countdown.setRect(0, 512, 256, 256, 0);
		}
		else if (currentTime > countdownTime + 2000) {
			Countdown.setRect(0, 768, 256, 512, 0);
		}
		else if (currentTime > countdownTime + 1000) {
			Countdown.setRect(0, 1024, 256, 768, 0);
		}
		else {
			Countdown.setRect(0, 256, 512, 0, 256);
		}

		calcFPS();
		updateTitle();
		figureDraw();
		//Background->Draw();
		Notes.Draw();
		Meter.Draw();
		sprite.Draw();
		Countdown.Draw();

		//LR同時押しでPause画面に戻る
		if ((inputDevice->getPushState(8, 0) && (inputDevice->getPushState(9, 0)))) {
			stateRestart = false;
			pauseTime += currentTime - countdownTime;
			countdownTime = timeGetTime();
		}
	}
}