#include "TitleRenderClass.h"
#include "InputDevice.h"

extern BYTE FrameNumber;
extern Device::InputDevice * inputDevice;

using namespace DxSprite;

namespace Frame {
	//コンストラクタ
	TitleRender::TitleRender() :
			Title      (_T("Data/img/opening/title.png")),
			Black      (_T("Data/img/color/black.png")),
			flag       (false) {
		Title.setPosition(0, 0.0f, 0.0f);
		Black.setPosition(0, 0.0f, 0.0f);
		Black.setRect(0, 1024, 768);
		//og = new OggPlay("Data/bgm/opening/title.ogg");
	}

	//デストラクタ
	TitleRender::~TitleRender() {
		//delete og;
	}

	//Rend関数
	void TitleRender::Rend(void) {
		Sprite->Begin(NULL);

		Title.Draw();
		Black.Draw();

		if (!flag) {
			if (!Black.addAlpha(-5)) flag = true;
		}
		else {
			//if (og->getState() != Dix::PCMPlayer::STATE_PLAY) og->Play(true);
		}

		inputDevice->getPushState();
		if (inputDevice->getPushStateAny(1)) FrameNumber = SONG_SELECT_INIT;

		Sprite->End();
	}
}