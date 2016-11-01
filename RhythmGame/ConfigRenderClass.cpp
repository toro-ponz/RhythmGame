#include "ConfigRenderClass.h"
#include "InputDevice.h"

extern BYTE FrameNumber;
extern Device::InputDevice * inputDevice;
extern HWND hWnd;

using namespace DxSprite;

namespace Frame {
	//コンストラクタ
	ConfigRender::ConfigRender() :
			Title     (_T("Data/img/config/title.png")),
			Buttons   (_T("Data/img/config/buttons.png")) {
		//Sprite[0].setPosition(0.0, 0.0);
		//Sprite[1].setPosition(512.0, -70.0);
		//Sprite[2].setPosition(768.0, -70.0);
		//Sprite[3].setPosition(845.0, 256.0);
		//Sprite[4].setPosition(845.0, 512.0);
	}

	//デストラクタ
	ConfigRender::~ConfigRender() {

	}

	//Rend関数
	void ConfigRender::Rend(void) {
		Sprite->Begin(NULL);

		//Sprite[0].Draw();
		//Sprite[1].Draw();
		//Sprite[2].Draw();
		//Sprite[3].Draw();
		//Sprite[4].Draw();

		inputDevice->getPushState();
		if (inputDevice->getPushStateAny(1)) FrameNumber = SONG_SELECT_INIT;

		Sprite->End();
	}
}