#include "ConfigRenderClass.h"
#include "InputDeviceClass.h"

extern BYTE g_FrameNo;
extern Device::InputDevice * inputDevice;
extern HWND hWnd;

using namespace DxSprite;

namespace Frame {
	//コンストラクタ
	ConfigRender::ConfigRender() {
		Sprite = new SpriteDrawing[5];
		Sprite[0].setTexture(_T("Data/img/config/config.png"));
		Sprite[1].setTexture(_T("Data/img/config/bt_ip.png"));
		Sprite[2].setTexture(_T("Data/img/config/bt_in.png"));
		Sprite[3].setTexture(_T("Data/img/config/bt_save.png"));
		Sprite[4].setTexture(_T("Data/img/config/bt_back.png"));
		Sprite[0].setPosition(0.0, 0.0);
		Sprite[1].setPosition(512.0, -70.0);
		Sprite[2].setPosition(768.0, -70.0);
		Sprite[3].setPosition(845.0, 256.0);
		Sprite[4].setPosition(845.0, 512.0);
	}

	//デストラクタ
	ConfigRender::~ConfigRender() {
		delete[] Sprite;
	}

	//Rend関数
	void ConfigRender::Rend(void) {
		g_pSprite->Begin(NULL);

		Sprite[0].Draw();
		Sprite[1].Draw();
		Sprite[2].Draw();
		Sprite[3].Draw();
		Sprite[4].Draw();

		inputDevice->getPushState();
		if (inputDevice->getPushStateAny(1)) g_FrameNo = SONG_SELECT_INIT;

		g_pSprite->End();
	}
}