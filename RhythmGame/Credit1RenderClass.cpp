#include "Credit1RenderClass.h"
#include "InputDeviceClass.h"

extern BYTE g_FrameNo;
extern Device::InputDevice * inputDevice;
extern HWND hWnd;

using namespace DxSprite;

namespace Frame {
	//コンストラクタ
	Credit1Render::Credit1Render():
			x        (0),
			flag     (false) {
		Sprite = new SpriteDrawing[2];
		Sprite[0].setTexture(_T("Data/img/credit/logo1.png"));
		Sprite[1].setTexture(_T("Data/img/color/black.png"));
		Sprite[1].setRect(1024, 768);
		Sprite[0].setPosition(256.0, 320.0) ;
		//wv = new WavPlay(_T("Data/se/credit/logo1.wav"));
	}

	//デストラクタ
	Credit1Render::~Credit1Render() {
		delete[] Sprite;
		//delete wv;
	}

	//Rend関数
	void Credit1Render::Rend(void) {
		g_pSprite->Begin(NULL);

		Sprite[0].Draw();
		Sprite[1].Draw();

		if (!flag) {
			if (!Sprite[1].addAlpha(-5)) flag = true;
		}
		else {
			if (x == 0 && flag) {
				//wv->Play();
			}
			x++;
			if (x > 100) {
				if (!Sprite[1].addAlpha(5)) g_FrameNo = TITLE_INIT;
			}
		}

		inputDevice->getPushState();
		if (inputDevice->getPushStateAny(1)) g_FrameNo = TITLE_INIT;

		g_pSprite->End();
	}
}