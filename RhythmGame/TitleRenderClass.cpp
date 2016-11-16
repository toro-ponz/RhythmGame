#include "TitleRenderClass.h"
#include "GlobalVariable.h"

namespace Frame {
	//コンストラクタ
	TitleRender::TitleRender() :
			Title(openingImageDirectoryPath + "title.png"),
			Black(colorDirectoryPath + "black.png") {
		Black.setRectFromPixel(0, 0, 768, 0, 1024);
		oldTime = timeGetTime();
	}

	//デストラクタ
	TitleRender::~TitleRender() {

	}

	//Rend関数
	void TitleRender::Rend(void) {
		Sprite->Begin(NULL);

		DWORD time = timeGetTime();
		inputDevice->getPushState();

		Title.Draw();
		Black.Draw();


		if (oldTime + 4 < time) {
			for (int i = 0; i < (int)((time - oldTime) / 4); i++) {
				Black.addAlpha(0, -1);
			}
			oldTime = time;
		}

		if (inputDevice->getPushStateAny(InputDevice::KEY_STATE::STATE_PUSH))
			frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::SONG_SELECT_INIT);

		Sprite->End();
	}
}