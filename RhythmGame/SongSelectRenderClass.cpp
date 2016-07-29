#include "SongSelectRenderClass.h"
#include "InputDeviceClass.h"

extern BYTE g_FrameNo;
extern Device::InputDevice * inputDevice;
extern HWND hWnd;

using namespace DxSprite;

namespace Frame {
	//コンストラクタ
	SongSelectRender::SongSelectRender() {
		Sprite = new DxSprite::SpriteDrawing[9];
		Sprite[0].setTexture(_T("Data/img/songselect/songselect.png"));
		Sprite[1].setTexture(_T("Data/img/songselect/bt_conf.png"));
		Sprite[2].setTexture(_T("Data/img/songselect/bt_exit.png"));
		Sprite[3].setTexture(_T("Data/img/songselect/frame1.png"));
		Sprite[4].setTexture(_T("Data/img/songselect/frame2.png"));
		Sprite[5].setTexture(_T("Data/img/songselect/bar.png"));
		Sprite[6].setTexture(_T("Data/img/color/black.png"));
		Sprite[7].setTexture(_T("Data/img/songselect/level.png"));
		Sprite[8].setTexture(_T("Data/img/songselect/cursor.png"));
		Sprite[6].setRect(1024, 1024);
		Sprite[1].setPosition(845.0, 256.0);
		Sprite[2].setPosition(845.0, 512.0);
		Sprite[3].setPosition(0.0, 328.0);
		Sprite[5].setPosition(90.0, 0.0);
		Sprite[6].setPosition(0.0, 0.0);
		Sprite[7].setPosition(300.0, 156.0);
		Sprite[8].setPosition(320.0, 176.0);
		Sprite[6].setColor(128, 255, 255, 255);
		ftc1 = new FontTextureCreate(_T("・Sample"), 60, 0, _T(""), 150.0f, 400.0f, false);
	}

	//デストラクタ
	SongSelectRender::~SongSelectRender() {
		delete[] Sprite;
		delete ftc1;
	}

	//Rend関数
	void SongSelectRender::Rend(void) {
		g_pSprite->Begin(NULL);

		inputDevice->getPushState();
		if (SCE == 0) {
			if (inputDevice->getPushState(0, 1)) {
				//sNum--;
			}
			if (inputDevice->getPushState(1, 1)) {
				//sNum++;
			}
			if (inputDevice->getPushState(2, 1)) {
				Sprite[1].setPosition(845.0 - 50.0, 256.0, 0.0);
				Sprite[2].setPosition(845.0, 512.0, 0.0);
				SCE = 1;
			}
			if (inputDevice->getPushState(3, 1)) {
				//
			}
		}
		else if (SCE == 1 || SCE == 2){
			if (inputDevice->getPushState(0, 1)) {
				Sprite[1].setPosition(845.0 - 50.0, 256.0, 0.0);
				Sprite[2].setPosition(845.0, 512.0, 0.0);
				SCE = 1;
			}
			if (inputDevice->getPushState(1, 1)) {
				Sprite[1].setPosition(845.0, 256.0, 0.0);
				Sprite[2].setPosition(845.0 - 50.0, 512.0, 0.0);
				SCE = 2;
			}
			if (inputDevice->getPushState(2, 1)) {

			}
			if (inputDevice->getPushState(3, 1)) {
				Sprite[1].setPosition(845.0, 256.0, 0.0);
				Sprite[2].setPosition(845.0, 512.0, 0.0);
				SCE = 0;
			}
		}
		else {
			if (inputDevice->getPushState(0, 1)) {
				if (level != 1) level--;
			}
			if (inputDevice->getPushState(1, 1)) {
				if (level != 3) level++;
			}
			if (inputDevice->getPushState(5, 1)) {
				level = 1;
				SCE = 0;
			}
			Sprite[8].setPosition(320.0, (float)(176.0 + (130.0 * (level - 1))), 0.0);
		}
		if (inputDevice->getPushState(4, 1)) {
			if (SCE == 0) {
				//レベル選択画面へ
				SCE = 3;
			}
			else if (SCE == 1) g_FrameNo = CONFIG_INIT;
			else if (SCE == 2) g_FrameNo = 255;
			else {
				if (level == 1) g_FrameNo = PLAY_EASY_INIT;
				else if (level == 2) g_FrameNo = PLAY_NORMAL_INIT;
				else g_FrameNo = PLAY_HARD_INIT;
			}
		}

		Sprite[0].Draw();
		Sprite[1].Draw();
		Sprite[2].Draw();
		Sprite[4].setPosition(0.0, 145.0);
		Sprite[4].Draw();
		Sprite[4].setPosition(0.0, 260.0);
		Sprite[4].Draw();
		Sprite[4].setPosition(0.0, 525.0);
		Sprite[4].Draw();
		Sprite[4].setPosition(0.0, 650.0);
		Sprite[4].Draw();
		if (SCE == 0 || SCE == 4) Sprite[3].Draw();
		else {
			Sprite[4].setPosition(0.0, 390.0, 0.0);
			Sprite[4].Draw();
		}
		Sprite[5].Draw();

		ftc1->Rend();

		if (SCE == 3) {
			Sprite[6].Draw();
			Sprite[7].Draw();
			Sprite[8].Draw();
		}

		g_pSprite->End();
	}
}