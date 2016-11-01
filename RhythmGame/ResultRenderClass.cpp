#include "ResultRenderClass.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

#define SONG_SELECT_INIT     25

extern BYTE FrameNumber;
extern Device::InputDevice * inputDevice;

namespace Frame {
	ResultRender::ResultRender(int miss, int bad, int nice, int good, int great, int combo, int score) :
			Result            (_T("Data/img/result/num.png"), 39) {
		result[0] = miss;
		result[1] = bad;
		result[2] = nice;
		result[3] = good;
		result[4] = great;
		result[5] = combo;
		result[6] = score;
		countUp = 0;

		Result.setTile(64.0f);
		for (int i = 0; i < 32; i++) {
			Result.setRectC(i, 1, 1);
		}
		Result.setRectC(32, 17, 19);
		Result.setRectC(33, 25, 27);
		Result.setRectC(34, 33, 35);
		Result.setRectC(35, 41, 43);
		Result.setRectC(36, 49, 51);
		Result.setRectC(37, 57, 59);
		Result.setRectC(38, 20, 22);

		for (int i = 0; i < 4; i++) {
			Result.setPosition(i, i * 50.0f + 512.0f, 64.0f * 2, 0.0f);
		}
		for (int i = 0; i < 4; i++) {
			Result.setPosition(i + 4, i * 50.0f + 512.0f, 64.0f * 3, 0.0f);
		}
		for (int i = 0; i < 4; i++) {
			Result.setPosition(i + 8, i * 50.0f + 512.0f, 64.0f * 4, 0.0f);
		}
		for (int i = 0; i < 4; i++) {
			Result.setPosition(i + 12, i * 50.0f + 512.0f, 64.0f * 5, 0.0f);
		}
		for (int i = 0; i < 4; i++) {
			Result.setPosition(i + 16, i * 50.0f + 512.0f, 64.0f * 6, 0.0f);
		}
		for (int i = 0; i < 4; i++) {
			Result.setPosition(i + 20, i * 50.0f + 512.0f, 64.0f * 7, 0.0f);
		}
		for (int i = 0; i < 8; i++) {
			Result.setPosition(i + 24, i * 50.0f + 512.0f, 64.0f * 8, 0.0f);
		}
		Result.setPosition(32, 256.0f, 128.0f, 0.0f);
		Result.setPosition(33, 256.0f, 192.0f, 0.0f);
		Result.setPosition(34, 256.0f, 256.0f, 0.0f);
		Result.setPosition(35, 256.0f, 320.0f, 0.0f);
		Result.setPosition(36, 256.0f, 384.0f, 0.0f);
		Result.setPosition(37, 256.0f, 448.0f, 0.0f);
		Result.setPosition(38, 256.0f, 512.0f, 0.0f);
	}

	ResultRender::~ResultRender() {

	}

	void ResultRender::Rend() {
		Sprite->Begin(NULL);
		inputDevice->getPushState();
		int digits = 4;

		if (countUp < 7) {
			if (inputDevice->getPushState(4, 1)) {
				count[countUp] = result[countUp];
				if (countUp == 6) digits = 8;
				else digits = 4;
				std::ostringstream sout;
				sout << std::setfill('0') << std::setw(digits) << count[countUp];
				std::string s = sout.str();
				for (int i = 0; i < digits; i++) {
					switch (s[i]) {
					case '0':
						Result.setRectC(i + (countUp * 4), 1, 1);
						break;
					case '1':
						Result.setRectC(i + (countUp * 4), 2, 2);
						break;
					case '2':
						Result.setRectC(i + (countUp * 4), 3, 3);
						break;
					case '3':
						Result.setRectC(i + (countUp * 4), 4, 4);
						break;
					case '4':
						Result.setRectC(i + (countUp * 4), 5, 5);
						break;
					case '5':
						Result.setRectC(i + (countUp * 4), 9, 9);
						break;
					case '6':
						Result.setRectC(i + (countUp * 4), 10, 10);
						break;
					case '7':
						Result.setRectC(i + (countUp * 4), 11, 11);
						break;
					case '8':
						Result.setRectC(i + (countUp * 4), 12, 12);
						break;
					case '9':
						Result.setRectC(i + (countUp * 4), 13, 13);
						break;
					}
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
				else digits = 4;
				std::ostringstream sout3;
				sout3 << std::setfill('0') << std::setw(digits) << count[countUp];
				std::string s = sout3.str();
				for (int i = 0; i < digits; i++) {
					switch (s[i]) {
					case '0':
						Result.setRectC(i + (countUp * 4), 1, 1);
						break;
					case '1':
						Result.setRectC(i + (countUp * 4), 2, 2);
						break;
					case '2':
						Result.setRectC(i + (countUp * 4), 3, 3);
						break;
					case '3':
						Result.setRectC(i + (countUp * 4), 4, 4);
						break;
					case '4':
						Result.setRectC(i + (countUp * 4), 5, 5);
						break;
					case '5':
						Result.setRectC(i + (countUp * 4), 9, 9);
						break;
					case '6':
						Result.setRectC(i + (countUp * 4), 10, 10);
						break;
					case '7':
						Result.setRectC(i + (countUp * 4), 11, 11);
						break;
					case '8':
						Result.setRectC(i + (countUp * 4), 12, 12);
						break;
					case '9':
						Result.setRectC(i + (countUp * 4), 13, 13);
						break;
					}
				}
				if (result[countUp] <= count[countUp]) {
					countUp++;
				}
			}
/*
			if (countUp == 6) digits = 8;
			else digits = 4;

			std::ostringstream sout2;
			sout2 << std::setfill('0') << std::setw(digits) << count[countUp];
			std::string s = sout2.str();
			for (int i = 0; i < digits; i++) {
				switch (s[i]) {
				case '0':
					Result.setRectC(i + (countUp * 4), 1, 1);
					break;
				case '1':
					Result.setRectC(i + (countUp * 4), 2, 2);
					break;
				case '2':
					Result.setRectC(i + (countUp * 4), 3, 3);
					break;
				case '3':
					Result.setRectC(i + (countUp * 4), 4, 4);
					break;
				case '4':
					Result.setRectC(i + (countUp * 4), 5, 5);
					break;
				case '5':
					Result.setRectC(i + (countUp * 4), 9, 9);
					break;
				case '6':
					Result.setRectC(i + (countUp * 4), 10, 10);
					break;
				case '7':
					Result.setRectC(i + (countUp * 4), 11, 11);
					break;
				case '8':
					Result.setRectC(i + (countUp * 4), 12, 12);
					break;
				case '9':
					Result.setRectC(i + (countUp * 4), 13, 13);
					break;
				}
			}*/
		}
		else {
			if (inputDevice->getPushState(4, 1)) {
				FrameNumber = SONG_SELECT_INIT;
			}
		}

		Result.Draw();
		Sprite->End();
	}
}