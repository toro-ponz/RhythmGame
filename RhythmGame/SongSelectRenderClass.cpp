#include "SongSelectRenderClass.h"
#include "InputDevice.h"

extern BYTE FrameNumber;
extern Device::InputDevice * inputDevice;

using namespace DxSprite;

namespace Frame {
	//コンストラクタ
	SongSelectRender::SongSelectRender() :
			Title        (_T("Data/img/songselect/songselect.png")),
			Buttons      (_T("Data/img/songselect/buttons.png")),
			Back         (_T("Data/img/songselect/frame2.png")),
			Level        (_T("Data/img/songselect/level.png")),
			Cursor       (_T("Data/img/songselect/cursor.png")),
			Black        (_T("Data/img/color/black.png")) {
		Black.setRect(0, 1024, 1024);
		Buttons.setPosition(0, 845.0, 256.0);
		//Buttons.setPosition(1, 845.0, 512.0);
		Back.setPosition(0, 0.0, 328.0);
		//Back.setPosition(1, 90.0, 0.0);
		Black.setPosition(0, 0.0, 0.0);
		Level.setPosition(0, 300.0, 156.0);
		Cursor.setPosition(0, 320.0, 176.0);
		Black.setColor(D3DCOLOR_ARGB(128, 255, 255, 255), 0);
		//ftc1 = new FontTextureCreate(_T("・Sample"), 60, 0, _T(""), 150.0f, 400.0f, false);
	}

	//デストラクタ
	SongSelectRender::~SongSelectRender() {
		//delete ftc1;
	}

	//Rend関数
	void SongSelectRender::Rend(void) {
		Sprite->Begin(NULL);

		inputDevice->getPushState();
		if (SCE == 0) {
			if (inputDevice->getPushState(0, 1)) {
				//sNum--;
			}
			if (inputDevice->getPushState(1, 1)) {
				//sNum++;
			}
			if (inputDevice->getPushState(2, 1)) {
				Buttons.setPosition(0, 845.0 - 50.0, 256.0, 0.0);
				//Buttons.setPosition(1, 845.0, 512.0, 0.0);
				SCE = 1;
			}
			if (inputDevice->getPushState(3, 1)) {
				//
			}
		}
		else if (SCE == 1 || SCE == 2){
			if (inputDevice->getPushState(0, 1)) {
				Buttons.setPosition(0, 845.0 - 50.0, 256.0, 0.0);
				Buttons.setPosition(1, 845.0, 512.0, 0.0);
				SCE = 1;
			}
			if (inputDevice->getPushState(1, 1)) {
				Buttons.setPosition(0, 845.0, 256.0, 0.0);
				Buttons.setPosition(1, 845.0 - 50.0, 512.0, 0.0);
				SCE = 2;
			}
			if (inputDevice->getPushState(2, 1)) {

			}
			if (inputDevice->getPushState(3, 1)) {
				Buttons.setPosition(0, 845.0, 256.0, 0.0);
				Buttons.setPosition(1, 845.0, 512.0, 0.0);
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
			Cursor.setPosition(0, 320.0, (float)(176.0 + (130.0 * (level - 1))), 0.0);
		}
		if (inputDevice->getPushState(4, 1)) {
			if (SCE == 0) {
				//レベル選択画面へ
				SCE = 3;
			}
			else if (SCE == 1) FrameNumber = CONFIG_INIT;
			else if (SCE == 2) FrameNumber = 255;
			else {
				if (level == 1) FrameNumber = PLAY_EASY_INIT;
				else if (level == 2) FrameNumber = PLAY_NORMAL_INIT;
				else FrameNumber = PLAY_HARD_INIT;
			}
		}

		Title.Draw();
		Buttons.Draw();
		Buttons.Draw();
		Back.setPosition(0, 0.0, 145.0);
		Back.Draw();
		Back.setPosition(0, 0.0, 260.0);
		Back.Draw();
		Back.setPosition(0, 0.0, 525.0);
		Back.Draw();
		Back.setPosition(0, 0.0, 650.0);
		Back.Draw();
		if (SCE == 0 || SCE == 4) Back.Draw();
		else {
			Back.setPosition(0, 0.0, 390.0, 0.0);
			Back.Draw();
		}
		Back.Draw();

		//ftc1->Rend();

		if (SCE == 3) {
			Black.Draw();
			Level.Draw();
			Cursor.Draw();
		}

		Sprite->End();
	}
}