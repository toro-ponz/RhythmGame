#define _CRT_SECURE_NO_WARNINGS

#include "PlayRenderClass.h"
#include <time.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <iomanip>

extern BYTE g_FrameNo;
extern HWND hWnd;
extern InputDevice * inputDevice;

using namespace DxSprite;

namespace Frame {
	//コンストラクタ
	PlayRender::PlayRender(int lv) :
			Sprite		(_T("Data/img/play/chip01.png"), 29, 32.0f),
			Meter		(1),
			Pau			(1),
			Countdown	(1) {
		string sheetFile = "Data/songs/Sample/Sample_" + to_string(lv) + ".dat";
		ifstream ifs(sheetFile);

		string str, tmp;
		getline(ifs, str);
		istringstream stream(str);
		getline(stream, songName, ',');
		getline(stream, fileName, ',');
		getline(stream, tmp, ',');
		level = atoi(tmp.c_str());
		getline(stream, tmp, ',');
		bpm = atoi(tmp.c_str());
		getline(stream, tmp, ',');
		rhythm = atoi(tmp.c_str());
		getline(stream, tmp, ',');
		speed = atoi(tmp.c_str());
		getline(stream, tmp, ';');
		datacnt = atoi(tmp.c_str());

		num.reserve(datacnt);
		mode.reserve(datacnt);
		dist1.reserve(datacnt);
		dist2.reserve(datacnt);
		nodule.reserve(datacnt);
		rhythmnum.reserve(datacnt);
		mom.reserve(datacnt);
		son.reserve(datacnt);

		Notes = new DxSprite::SpriteDrawing2(datacnt);
		Notes->setTexture(_T("Data/img/play/notes.png"));

		for (int i = 0; i < datacnt; i++) {
			getline(ifs, str);
			istringstream stream(str);
			getline(stream, tmp, ',');
			num.push_back(atoi(tmp.c_str()));
			getline(stream, tmp, ',');
			mode.push_back(atoi(tmp.c_str()));
			getline(stream, tmp, ',');
			dist1.push_back(atoi(tmp.c_str()));
			getline(stream, tmp, ',');
			dist2.push_back(atoi(tmp.c_str()));
			getline(stream, tmp, ',');
			nodule.push_back(atoi(tmp.c_str()));
			getline(stream, tmp, ',');
			rhythmnum.push_back(atoi(tmp.c_str()));
			getline(stream, tmp, ',');
			mom.push_back(atoi(tmp.c_str()));
			getline(stream, tmp, ';');
			son.push_back(atoi(tmp.c_str()));
			Notes->setCenter(i, 32.0f, 32.0f);
		}

		Sprite.setRectC(0, 1, 740);
		Sprite.setRect(1, 256, 768, 128, 0);
		Sprite.setRect(2, 32, 800, 0, 768);
		Sprite.setRect(3, 32, 800, 0, 768);
		Sprite.setRect(4, 32, 800, 0, 768);
		Sprite.setRect(5, 32, 800, 0, 768);
		Sprite.setRect(6, 128, 896, 0, 864);
		Sprite.setRect(7, 32, 928, 0, 896);
		Sprite.setRect(8, 32, 928, 0, 896);
		Sprite.setRect(9, 32, 928, 0, 896);
		Sprite.setRect(10, 32, 928, 0, 896);
		Sprite.setRect(11, 32, 928, 0, 896);
		Sprite.setRect(12, 32, 928, 0, 896);
		Sprite.setRect(13, 32, 928, 0, 896);
		Sprite.setRect(14, 32, 928, 0, 896);
		Sprite.setRect(15, 32, 1024, 0, 992);
		Sprite.setRect(16, 64, 1024, 32, 992);
		Sprite.setRect(17, 96, 1024, 64, 992);
		Sprite.setRect(18, 128, 1024, 96, 992);
		Sprite.setRect(19, 96, 992, 64, 960);
		Sprite.setRect(20, 128, 992, 96, 960);
		Sprite.setRect(21, 256, 1024, 128, 960);
		Sprite.setRect(22, 1024, 768, 256, 0);
		Sprite.setRect(23, 384, 896, 256, 768);
		Sprite.setRect(24, 384, 896, 256, 768);
		Sprite.setRect(25, 384, 896, 256, 768);
		Sprite.setRect(26, 384, 896, 256, 768);
		Sprite.setRect(27, 384, 896, 256, 768);
		Sprite.setRect(28, 384, 896, 256, 768);
		Sprite.setPosition(0, 0.0f, 0.0f);
		Sprite.setPosition(1, 896.0f, 0.0f);
		for (int i = 0; i < 5; i++) {
			Sprite.setPosition(i + 2, (float)(650.0 + (i * 23.0)), 580.0);
		}
		for (int i = 0; i < 8; i++) {
			Sprite.setPosition(i + 7, (float)(700.0 + (i * 20.0)), (float)(768.0 - 40.0));
		}
		Sprite.setPosition(15, 128.0, 700.0);
		Sprite.setPosition(16, 128.0 + 32.0, 700.0);
		Sprite.setPosition(17, 128.0 + 64.0, 700.0);
		Sprite.setPosition(18, 128.0 + 96.0, 700.0);
		Sprite.setPosition(19, 128.0 + 128.0, 700.0);
		Sprite.setPosition(20, 128.0 + 160.0, 700.0);
		Sprite.setPosition(21, 768.0, 512.0);
		Sprite.setPosition(22, 128.0f, 0.0f);

		Sprite.setPosition(23, 512.0f, 352.0f);
		Sprite.setPosition(24, 512.0f, 416.0f);
		Sprite.setPosition(25, 544.0f, 384.0f);
		Sprite.setPosition(26, 480.0f, 384.0f);
		Sprite.setPosition(27, 544.0f, 352.0f);
		Sprite.setPosition(28, 480.0f, 352.0f);

		Sprite.setCenter(23, 64.0f, 64.0f);
		Sprite.setCenter(24, 64.0f, 64.0f);
		Sprite.setCenter(25, 64.0f, 64.0f);
		Sprite.setCenter(26, 64.0f, 64.0f);
		Sprite.setCenter(27, 64.0f, 64.0f);
		Sprite.setCenter(28, 64.0f, 64.0f);

		Sprite.disableDraw(21);

		Meter.setTexture(_T("Data/img/color/red.png"));
		Meter.setCenter(0, 128.0, 768.0);
		Meter.setPosition(0, 1024.0, 768.0);
		Meter.putEnable(0);

		Pau.setTexture(_T("Data/img/play/pause.png"));
		Pau.setCenter(0, 256.0f, 128.0f);
		Pau.setPosition(0, 512.0f, 384.0f);
		Pau.putEnable(0);

		Countdown.setTexture(_T("Data/img/play/cntdwn.png"));
		Countdown.setCenter(0, 128.0f, 128.0f);
		Countdown.setPosition(0, 512.0f, 384.0f);
		Countdown.putEnable(0);

		TCHAR buf[100];
		GetPrivateProfileString(_T("common"), _T("autoplay"), _T("ON"), buf, sizeof(buf), _T("Data/conf.ini"));
		if (!wcscmp(buf, _T("ON"))) auto_play = true;
		else auto_play = false;

		string musicFile = "Data/songs/" + songName + "/" + fileName;
		Song = new OggPlay(musicFile.c_str());
		sTime = timeGetTime();
	}

	//デストラクタ
	PlayRender::~PlayRender() {
		delete Notes;
		delete Song;
	}

	//Rend関数
	void PlayRender::Rend(void) {
		g_pSprite->Begin(NULL);

		DWORD time = timeGetTime();
		inputDevice->getPushState();
		if (!pause) {
			Play(time);
		}
		else {
			if (!restart) Pause(time);
			else Restart(time);
		}
		oTime = time;

		g_pSprite->End();
	}

	int PlayRender::getResult(int mode) {
		int result;
		switch (mode) {
		case 0:
			result = miss;
			break;
		case 1:
			result = bad;
			break;
		case 2:
			result = nice;
			break;
		case 3:
			result = good;
			break;
		case 4:
			result = great;
			break;
		case 5:
			result = maxCombo;
			break;
		case 6:
			result = score;
			break;
		default:
			result = 0;
			break;
		}
		return result;
	}

	//Play関数
	void PlayRender::Play(DWORD time) {
		//speedms待ってから再生開始
		if (Song->getState() != Dix::PCMPlayer::STATE_PLAY && !flag && ((int)(time - sTime + pTime) >= speed)) {
			Song->Play(false);
			flag = true;
			sTime = timeGetTime() - speed;
		}

		DWORD songTime = (DWORD)((nodule[stop] - 1) * (60 * 1000) / bpm * 4);
		songTime += (DWORD)((rhythmnum[stop] - 1) * (60 * 1000 / bpm));
		if (son[stop] && mom[stop]) songTime += ((60 * 1000 / bpm) * son[stop] / mom[stop]);

		if (songTime <= (time - sTime - pTime)) {
			float plus = (float)((time - sTime - pTime - songTime) * 384.0 / speed);
			Notes->putEnable(stop);
			switch (dist1[stop]) {
			case 1:
				Notes->setPosition(stop, 512.0f, -32.0f + plus);
				break;
			case 2:
				Notes->setPosition(stop, 512.0f, (768.0f + 32.0f) - plus);
				break;
			case 3:
				Notes->setPosition(stop, (896.0f + 32.0f) - plus, 384.0f);
				break;
			case 4:
				Notes->setPosition(stop, (128.0f - 32.0f) + plus, 384.0f);
				break;
			case 5:
				Notes->setPosition(stop, 896.0f + 32.0f - plus, -32.0f + plus);
				break;
			case 6:
				Notes->setPosition(stop, 128.0f - 32.0f + plus, -32.0f + plus);
				break;
			}
			stop++;
		}
		JoyStick();
		if (start != stop) judge(time);
		calcFPS(time);
		updateTitle();
		numDraw();
		Notes->Draw();
		Meter.Draw();
		Sprite.Draw();

		oTime = time;
		//曲が終了したらリザルト画面のフレームへ
		if (flag && Song->getState() != Dix::PCMPlayer::STATE_PLAY && Song->getState() != Dix::PCMPlayer::STATE_PAUSE) {
			g_FrameNo = RESULT_INIT;
		}
	}

	//JoyStick関数
	void PlayRender::JoyStick() {
		for (int i = 0; i < 6; i++) {
			Sprite.disableDraw(15 + i);
			Sprite.disableDraw(23 + i);
		}
		if ((inputDevice->getPushState(0, 0) || (inputDevice->getPushState(6, 0)))) {
			Sprite.enableDraw(15);
		}
		if ((inputDevice->getPushState(1, 0) || (inputDevice->getPushState(5, 0)))) {
			Sprite.enableDraw(16);
		}
		if ((inputDevice->getPushState(2, 0) || (inputDevice->getPushState(4, 0)))) {
			Sprite.enableDraw(17);
		}
		if ((inputDevice->getPushState(3, 0) || (inputDevice->getPushState(7, 0)))) {
			Sprite.enableDraw(18);
		}
		if (inputDevice->getPushState(8, 0)) {
			Sprite.enableDraw(19);
		}
		if (inputDevice->getPushState(9, 0)) {
			Sprite.enableDraw(20);
		}

		if ((inputDevice->getPushState(0, 1) || (inputDevice->getPushState(6, 1)))) {
			Sprite.enableDraw(23);
		}
		if ((inputDevice->getPushState(1, 1) || (inputDevice->getPushState(5, 1)))) {
			Sprite.enableDraw(24);
		}
		if ((inputDevice->getPushState(2, 1) || (inputDevice->getPushState(4, 1)))) {
			Sprite.enableDraw(25);
		}
		if ((inputDevice->getPushState(3, 1) || (inputDevice->getPushState(7, 1)))) {
			Sprite.enableDraw(26);
		}
		if (inputDevice->getPushState(9, 1)) {
			Sprite.enableDraw(27);
		}
		if (inputDevice->getPushState(8, 1)) {
			Sprite.enableDraw(28);
		}

		//START又はSELECT押下で終了
		if ((inputDevice->getPushState(10, 1) || (inputDevice->getPushState(11, 1)))) {
			pause = true;
		}
	}

	//judge関数
	void PlayRender::judge(DWORD time) {
		int judge = 0; //1:MISS, 2:BAD, 3:NICE, 4:GOOD, 5:GREAT
		const float distx = 512.0;
		const float disty = 384.0;
		const float greatRange = 0.4f;
		const float goodRange = 0.6f;
		const float niceRange = 0.75f;
		const float badRange = 1.0f;
		const float out = (float)(32.0 * 1000 / speed);

		const float points[6] = { 352.0f, 416.0f, 544.0f, 480.0f, 352.0f, 352.0f };

		float point;
		float p_great = out * greatRange;
		float p_good = out * goodRange;
		float p_nice = out * niceRange;
		float p_bad = out * badRange;

		float add = (float)((DWORD)(time - oTime) * 384.0 / speed);
		float pos;

		bool push_j[6] = { false, false, false, false, false, false };

		for (int i = start; i < stop; i++) {
			bool push = false;
			point = points[dist1[i] - 1];
			//オートプレイ
			if (auto_play) {
				switch (dist1[i]) {
				case 1:
					pos = Notes->addPositionY(i, add);
					if (pos >= point) push = true;
					break;
				case 2:
					pos = Notes->addPositionY(i, -add);
					if (pos <= point) push = true;
					break;
				case 3:
					pos = Notes->addPositionX(i, -add);
					if (pos <= point) push = true;
					break;
				case 4:
					pos = Notes->addPositionX(i, add);
					if (pos >= point) push = true;
					break;
				case 5:
					Notes->addPositionX(i, -add);
					pos = Notes->addPositionY(i, add);
					if (pos >= point) push = true;
					break;
				case 6:
					Notes->addPositionX(i, add);
					pos = Notes->addPositionY(i, add);
					if (pos >= point) push = true;
					break;
				}
			}
			else {
				switch (dist1[i]) {
				case 1:
					pos = Notes->addPositionY(i, add);
					if ((pos > point - out) && (inputDevice->getPushState(0, 1) || inputDevice->getPushState(6, 1)) && !push_j[dist1[i] - 1]) {
						push = true;
						push_j[dist1[i] - 1] = true;
					}
					else if (pos > point + out) {
						push = true;
					}
					break;
				case 2:
					pos = Notes->addPositionY(i, -add);
					if ((pos < point + out) && (inputDevice->getPushState(1, 1) || inputDevice->getPushState(5, 1)) && !push_j[dist1[i] - 1]) {
						push = true;
						push_j[dist1[i] - 1] = true;
					}
					else if (pos < point - out) {
						push = true;
					}
					break;
				case 3:
					pos = Notes->addPositionX(i, -add);
					if ((pos < point + out) && (inputDevice->getPushState(2, 1) || inputDevice->getPushState(4, 1)) && !push_j[dist1[i] - 1]) {
						push = true;
						push_j[dist1[i] - 1] = true;
					}
					else if (pos < point - out) {
						push = true;
					}
					break;
				case 4:
					pos = Notes->addPositionX(i, add);
					if ((pos > point - out) && ((inputDevice->getPushState(3, 1) || inputDevice->getPushState(7, 1))) && !push_j[dist1[i] - 1]) {
						push = true;
						push_j[dist1[i] - 1] = true;
					}
					else if (pos > point + out) {
						push = true;
					}
					break;
				case 5:
					Notes->addPositionX(i, -add);
					pos = Notes->addPositionY(i, add);
					if ((pos >= point - out) && inputDevice->getPushState(9, 1) && !push_j[dist1[i] - 1]) {
						push = true;
						push_j[dist1[i] - 1] = true;
					}
					else if (pos > point + out) {
						push = true;
					}
					break;
				case 6:
					Notes->addPositionX(i, add);
					pos = Notes->addPositionY(i, add);
					if ((pos >= point - out) && inputDevice->getPushState(8, 1) && !push_j[dist1[i] - 1]) {
						push = true;
						push_j[dist1[i] - 1] = true;
					}
					else if (pos > point + out) {
						push = true;
					}
					break;
				}
			}
			if (push) {
				if ((pos > point - p_great) && (pos < point + p_great)) {
					judge = 5;
					SE(_T("Data/se/play/bt1.wav"));
				}
				else if ((pos > point - p_good) && (pos < point + p_good)) {
					judge = 4;
					SE(_T("Data/se/play/bt1.wav"));
				}
				else if ((pos > point - p_nice) && (pos < point + p_nice)) {
					judge = 3;
					SE(_T("Data/se/play/bt1.wav"));
				}
				else if ((pos > point - p_bad) && (pos < point + p_bad)) {
					judge = 2;
					SE(_T("Data/se/play/bt1.wav"));
				}
				else {
					judge = 1;
				}
			}
		}
		Meter.setRect(0, 128, (LONG)(((great + (good * 0.7) + (nice * 0.4) - (bad * 0.5) - miss) / (datacnt * 0.85)) * 768));
		Meter.setCenter(0, 128.0, (float)(((great + (good * 0.7) + (nice * 0.4) - (bad * 0.5) - miss) / (datacnt * 0.85)) * 768));
		switch (judge) {
		case 1: //MISSのとき
			Notes->putDisable(start);
			Sprite.disableDraw(21);
			start++;
			miss++;
			combo = 0;
			break;
		case 2: //BADのとき
			Notes->putDisable(start);
			Sprite.enableDraw(21);
			Sprite.setRect(21, 256, 832, 128, 768);
			start++;
			bad++;
			combo = 0;
			score += 50;
			break;
		case 3: //NICEのとき
			Notes->putDisable(start);
			Sprite.enableDraw(21);
			Sprite.setRect(21, 256, 896, 128, 832);
			start++;
			nice++;
			combo = 0;
			score += 150;
			break;
		case 4: //GOODのとき
			Notes->putDisable(start);
			Sprite.enableDraw(21);
			Sprite.setRect(21, 256, 960, 128, 896);
			start++;
			good++;
			combo++;
			break;
		case 5: //GREATのとき
			Notes->putDisable(start);
			Sprite.enableDraw(21);
			Sprite.setRect(21, 256, 1024, 128, 960);
			start++;
			great++;
			combo++;
			break;
		}
		if (judge > 3) {
			calcScore();
		}
	}

	//calcScore関数
	void PlayRender::calcScore() {
		float rate = 1.0f;
		if (combo == 100) score += 5000;
		else if (combo == 200) score += 10000;
		else if (combo == 300) score += 20000;
		else if (combo == 400) score += 40000;
		else if (combo == 500) score += 600000;
		else if (combo == 600) score += 100000;
		else if (combo == 700) score += 150000;
		else if (combo == 800) score += 200000;
		else if (combo == 900) score += 300000;
		else if (combo == 1000) score += 400000;
		else if (combo % 100 == 0) score += 500000;
		if (combo >= 1000) rate = 3.0f;
		else if (combo >= 900) rate = 2.75f;
		else if (combo >= 800) rate = 2.5f;
		else if (combo >= 700) rate = 2.25f;
		else if (combo >= 600) rate = 2.0f;
		else if (combo >= 500) rate = 1.75f;
		else if (combo >= 400) rate = 1.5f;
		else if (combo >= 300) rate = 1.35f;
		else if (combo >= 200) rate = 1.2f;
		else if (combo >= 100) rate = 1.1f;
		else if (combo >= 50) rate = 1.05f;
		score += (int)(500 * rate);
		if (score > 99999999) score = 99999999;
		if (combo > maxCombo) maxCombo = combo;
	}

	//calcFPS関数
	void PlayRender::calcFPS(DWORD time) {
		if (fpsTime <= time - 1000) {
			fps = cfps;
			cfps = 1;
			fpsTime = time;
		}
		else {
			cfps++;
		}
	}

	//updateTitle関数
	void PlayRender::updateTitle() {
		TCHAR lpsz[100] = _T("RhythmGame (Miss:");
		TCHAR xxx[12] = _T("00000");
		wcscat(lpsz, _itow(miss, xxx, 10));
		wcscat(lpsz, _T(", Bad:"));
		wcscat(lpsz, _itow(bad, xxx, 10));
		wcscat(lpsz, _T(", Nice:"));
		wcscat(lpsz, _itow(nice, xxx, 10));
		wcscat(lpsz, _T(", Good:"));
		wcscat(lpsz, _itow(good, xxx, 10));
		wcscat(lpsz, _T(", Great:"));
		wcscat(lpsz, _itow(great, xxx, 10));
		wcscat(lpsz, _T(",  / "));
		wcscat(lpsz, _itow(start, xxx, 10));
		wcscat(lpsz, _T(") FPS : "));
		wcscat(lpsz, _itow(fps, xxx, 10));
		::SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)lpsz);
	}

	//numDraw関数
	void PlayRender::numDraw() {
		std::ostringstream sout;
		sout << std::setfill('0') << std::setw(8) << score;
		std::string s = sout.str();
		for (int i = 0; i < 8; i++) {
			switch (s[i]) {
			case '0':
				Sprite.setRect(i + 7, 32, 928, 0, 896);
				break;
			case '1':
				Sprite.setRect(i + 7, 64, 928, 32, 896);
				break;
			case '2':
				Sprite.setRect(i + 7, 96, 928, 64, 896);
				break;
			case '3':
				Sprite.setRect(i + 7, 128, 928, 96, 896);
				break;
			case '4':
				Sprite.setRect(i + 7, 32, 960, 0, 928);
				break;
			case '5':
				Sprite.setRect(i + 7, 64, 960, 32, 928);
				break;
			case '6':
				Sprite.setRect(i + 7, 96, 960, 64, 928);
				break;
			case '7':
				Sprite.setRect(i + 7, 128, 960, 96, 928);
				break;
			case '8':
				Sprite.setRect(i + 7, 32, 992, 0, 960);
				break;
			case '9':
				Sprite.setRect(i + 7, 64, 992, 32, 960);
				break;
			}
		}
		std::ostringstream sout2;
		sout2 << std::setfill(' ') << std::setw(4) << combo;
		s = sout2.str();
		if (combo < 10) {
			s = "    ";
			Sprite.disableDraw(6);
		}
		else {
			Sprite.enableDraw(6);
		}
		for (int i = 0; i < 4; i++) {
			switch (s[i]) {
			case ' ':
				Sprite.setRect(i + 2, 96, 864, 64, 832);
				break;
			case '0':
				Sprite.setRect(i + 2, 32, 800, 0, 768);
				break;
			case '1':
				Sprite.setRect(i + 2, 64, 800, 32, 768);
				break;
			case '2':
				Sprite.setRect(i + 2, 96, 800, 64, 768);
				break;
			case '3':
				Sprite.setRect(i + 2, 128, 800, 96, 768);
				break;
			case '4':
				Sprite.setRect(i + 2, 32, 832, 0, 800);
				break;
			case '5':
				Sprite.setRect(i + 2, 64, 832, 32, 800);
				break;
			case '6':
				Sprite.setRect(i + 2, 96, 832, 64, 800);
				break;
			case '7':
				Sprite.setRect(i + 2, 128, 832, 96, 800);
				break;
			case '8':
				Sprite.setRect(i + 2, 32, 864, 0, 832);
				break;
			case '9':
				Sprite.setRect(i + 2, 64, 864, 32, 832);
				break;
			}
		}
	}

	//SE関数
	void PlayRender::SE(TCHAR * path) {
		PlaySound(path, NULL, SND_FILENAME | SND_ASYNC);
	}

	//Pause関数
	void PlayRender::Pause(DWORD time) {
		//再生を止める
		if (Song->getState() == Dix::PCMPlayer::STATE_PLAY) {
			Song->Pause();
			xTime = timeGetTime();
		}
		calcFPS(time);
		updateTitle();
		numDraw();
		//Background->Draw();
		Notes->Draw();
		Meter.Draw();
		Sprite.Draw();
		Pau.Draw();

		//STARTSELECT同時押しで再開カウントダウンへ
		if ((inputDevice->getPushState(10, 0) && (inputDevice->getPushState(11, 0)))) {
			restart = true;
			pTime += time - xTime;
			xTime = timeGetTime();
		}

		//LR同時押しでSONGSELECTへ
		if ((inputDevice->getPushState(8, 0) && (inputDevice->getPushState(9, 0)))) {
			g_FrameNo = SONG_SELECT_INIT;
		}
	}

	//Restart関数
	void PlayRender::Restart(DWORD time) {
		//5秒待って再開
		if (time > xTime + 5000) {
			pause = false;
			restart = false;
			Song->Play(false);
			pTime += time - xTime;
		}
		else if (time > xTime + 4000) {
			Countdown.setRect(0, 256, 256, 0, 0);
		}
		else if (time > xTime + 3000) {
			Countdown.setRect(0, 512, 256, 256, 0);
		}
		else if (time > xTime + 2000) {
			Countdown.setRect(0, 768, 256, 512, 0);
		}
		else if (time > xTime + 1000) {
			Countdown.setRect(0, 1024, 256, 768, 0);
		}
		else {
			Countdown.setRect(0, 256, 512, 0, 256);
		}

		calcFPS(time);
		updateTitle();
		numDraw();
		//Background->Draw();
		Notes->Draw();
		Meter.Draw();
		Sprite.Draw();
		Countdown.Draw();

		//LR同時押しでPause画面に戻る
		if ((inputDevice->getPushState(8, 0) && (inputDevice->getPushState(9, 0)))) {
			restart = false;
			pTime += time - xTime;
			xTime = timeGetTime();
		}
	}
}