#include "ResultRenderClass.h"
#include "GlobalVariable.h"

namespace Frame {
	/**
	*  コンストラクタ
	*/
	ResultRender::ResultRender(int miss, int bad, int nice, int good, int great, int combo, int score) :
			Result("Data/img/result/num.png", 39, 64.0f) {
		result[0] = miss;
		result[1] = bad;
		result[2] = nice;
		result[3] = good;
		result[4] = great;
		result[5] = combo;
		result[6] = score;
		countUp = 0;

		for (int i = 0; i < 32; i++) {
			Result.setRectFromChip(i, 1, 1);
		}
		Result.setRectFromChip(32, 13, 15);
		Result.setRectFromChip(33, 19, 21);
		Result.setRectFromChip(34, 25, 27);
		Result.setRectFromChip(35, 31, 33);
		Result.setRectFromChip(36, 37, 39);
		Result.setRectFromChip(37, 43, 45);
		Result.setRectFromChip(38, 16, 18);

		for (int i = 0; i < 4; i++) {
			Result.setPosition(i, i * 50.0f + 512.0f, 64.0f * 2, 0.0f);
			Result.setPosition(i + 4, i * 50.0f + 512.0f, 64.0f * 3, 0.0f);
			Result.setPosition(i + 8, i * 50.0f + 512.0f, 64.0f * 4, 0.0f);
			Result.setPosition(i + 12, i * 50.0f + 512.0f, 64.0f * 5, 0.0f);
			Result.setPosition(i + 16, i * 50.0f + 512.0f, 64.0f * 6, 0.0f);
			Result.setPosition(i + 20, i * 50.0f + 512.0f, 64.0f * 7, 0.0f);
			Result.setPosition(i + 24, i * 50.0f + 512.0f, 64.0f * 8, 0.0f);
			Result.setPosition(i + 28, (i + 4) * 50.0f + 512.0f, 64.0f * 8, 0.0f);
		}

		Result.setPosition(32, 256.0f, 128.0f, 0.0f);
		Result.setPosition(33, 256.0f, 192.0f, 0.0f);
		Result.setPosition(34, 256.0f, 256.0f, 0.0f);
		Result.setPosition(35, 256.0f, 320.0f, 0.0f);
		Result.setPosition(36, 256.0f, 384.0f, 0.0f);
		Result.setPosition(37, 256.0f, 448.0f, 0.0f);
		Result.setPosition(38, 256.0f, 512.0f, 0.0f);
	}

	/**
	*  デストラクタ
	*/
	ResultRender::~ResultRender() {

	}

	/**
	*  描画する関数.
	*/
	void ResultRender::Rend() {
		Sprite->Begin(NULL);
		inputDevice->getPushState();
		int digits = 4;

		if (countUp < 7) {
			if (inputDevice->getPushStateAny(InputDevice::KEY_STATE::STATE_PUSH)) {
				count[countUp] = result[countUp];
				if (countUp == 6) digits = 8;
				std::ostringstream sout;
				sout << std::setfill('0') << std::setw(digits) << count[countUp];
				std::string s = sout.str();
				for (int i = 0; i < digits; i++) {
					int figure = (int)(s[i] - '0');
					Result.setRectFromChip(i + (countUp * 4), figure + 1);
				}
				countUp++;
			}
			else {
				if (countUp < 6) {
					if (result[countUp] < 60) {
						count[countUp] += 1;
					}
					else {
						count[countUp] += result[countUp] / 60;
					}
				}
				else if (countUp == 6) {
					if (result[countUp] < 180) {
						count[countUp] += 1;
					}
					else {
						count[countUp] += result[countUp] / 180;
					}
				}
				if (result[countUp] <= count[countUp]) {
					count[countUp] = result[countUp];
				}
				if (countUp == 6) digits = 8;
				std::ostringstream sout;
				sout << std::setfill('0') << std::setw(digits) << count[countUp];
				std::string s = sout.str();
				for (int i = 0; i < digits; i++) {
					int figure = (int)(s[i] - '0');
					Result.setRectFromChip(i + (countUp * 4), figure + 1);
				}
				if (result[countUp] <= count[countUp]) {
					countUp++;
				}
			}
		}
		else {
			if (inputDevice->getPushState(InputDevice::KEY::BUTTON_RIGHT, InputDevice::KEY_STATE::STATE_PUSH) ||
				inputDevice->getPushState(InputDevice::KEY::RETURN, InputDevice::KEY_STATE::STATE_PUSH)) {
				frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::SONG_SELECT_INIT);
			}
		}

		Result.Draw();
		Sprite->End();
	}
}