#include "TitleRenderClass.h"
#include "Global.h"

namespace Frame {
	//�R���X�g���N�^
	TitleRender::TitleRender() :
			Background(imageDirectoryPath + "background.png"),
			Title(openingImageDirectoryPath + "title.png"),
			Black(colorDirectoryPath + "black.png") {
		Black.setRectFromPixel(0, 0, 768, 0, 1024);
		oldTime = timeGetTime();
	}

	//�f�X�g���N�^
	TitleRender::~TitleRender() {

	}

	//Rend�֐�
	void TitleRender::Rend(void) {
		Sprite->Begin(NULL);

		DWORD time = timeGetTime();
		inputDevice->getPushState();

		Background.Draw();
		Title.Draw();
		Black.Draw();


		if (oldTime + 4 < time) {
			for (int i = 0; i < (int)((time - oldTime) / 4); i++) {
				Black.addAlpha(0, -1);
			}
			oldTime = time;
		}

		if (inputDevice->getPushStateAny(InputDevice::KEY_STATE::STATE_PUSH)) {
			frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::SONG_SELECT_INIT);
			playSoundEffectFromResource(6);
		}

		Sprite->End();
	}
}