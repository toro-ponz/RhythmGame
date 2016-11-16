#include "Credit1RenderClass.h"
#include "InputDevice.h"
#include "GlobalVariable.h"

namespace Frame {
	/**
	*  コンストラクタ
	*/
	Credit1Render::Credit1Render() :
			Logo(creditImageDirectoryPath + "logo.png"),
			Black(colorDirectoryPath + "black.png") {
		Logo.setPosition(0, 256.0f, 320.0f);
		Black.setRectFromPixel(0, 0, 768, 0, 1024);
		state = 0;
		oldTime = timeGetTime();
	}

	/**
	*  デストラクタ
	*/
	Credit1Render::~Credit1Render() {

	}

	/**
	*  描画する関数.
	*/
	void Credit1Render::Rend(void) {
		Sprite->Begin(NULL);

		DWORD time = timeGetTime();
		inputDevice->getPushState();

		Logo.Draw();
		Black.Draw();


		switch (state) {
		case 0:
			if (oldTime + 4 < time) {
				for (int i = 0; i < (int)((time - oldTime) / 4); i++) {
					if (!Black.addAlpha(0, -1)) {
						state++;
						break;
					}
				}
				oldTime = time;
			}
			break;
		case 1:
			if (oldTime + 2000 < time) {
				oldTime = time;
				state++;
			}
			break;
		case 2:
			if (oldTime + 4 < time) {
				for (int i = 0; i < (int)((time - oldTime) / 4); i++) {
					if (!Black.addAlpha(0, 1)) {
						state++;
						break;
					}
				}
				oldTime = time;
			}
			break;
		case 3:
			frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::CREDIT2_INIT);
			break;
		}

		if (inputDevice->getPushStateAny(InputDevice::KEY_STATE::STATE_PUSH))
			frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::CREDIT2_INIT);

		Sprite->End();
	}
}