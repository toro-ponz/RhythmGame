#include "Credit2RenderClass.h"
#include "InputDevice.h"

extern BYTE FrameNumber;
extern Device::InputDevice * inputDevice;

using namespace DxSprite;

namespace Frame {
	//コンストラクタ
	Credit2Render::Credit2Render():
			Logo     (_T("Data/img/cledit/logo1.png")),
			Black    (_T("Data/img/color/black.png")),
			x        (0),
			flag     (false) {
		Logo.setPosition(0, 256.0f, 320.0f);
		Black.setPosition(0, 0.0f, 0.0f);
		Black.setRect(0, 1024, 768);
		wv = new WavPlayer(_T("Data/se/credit/logo2.wav"));
	}

	//デストラクタ
	Credit2Render::~Credit2Render() {
		delete[] Sprite;
		delete wv;
	}

	//Rend関数
	void Credit2Render::Rend(void) {
		Sprite->Begin(NULL);

		Logo.Draw();

		if (!flag) {
			if (!Black.addAlpha(-5)) flag = true;
		}
		else {
			if (x == 0 && flag) {
				//wv->Play();
			}
			x++;
			if (x > 100) {
				if (!Black.addAlpha(5)) FrameNumber = TITLE_INIT;
			}
		}

		inputDevice->getPushState();
		if (inputDevice->getPushStateAny(1)) FrameNumber = TITLE_INIT;

		Sprite->End();
	}
}