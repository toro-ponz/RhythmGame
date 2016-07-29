#include "TitleRenderClass.h"
#include "InputDeviceClass.h"

extern BYTE g_FrameNo;
extern Device::InputDevice * inputDevice;
extern HWND hWnd;

using namespace DxSprite;

namespace Frame {
	//コンストラクタ
	TitleRender::TitleRender() :
			flag       (false) {
		Sprite = new DxSprite::SpriteDrawing[2];
		Sprite[0].setTexture(_T("Data/img/opening/title.png"));
		Sprite[1].setTexture(_T("Data/img/color/black.png"));
		Sprite[1].setRect(1024, 1024);
		Sprite[0].setPosition(0.0f, 0.0f);
		Sprite[1].setPosition(0.0f, 0.0f);
		//og = new OggPlay("Data/bgm/opening/title.ogg");
	}

	//デストラクタ
	TitleRender::~TitleRender() {
		delete[] Sprite;
		//delete og;
	}

	//Rend関数
	void TitleRender::Rend(void) {
		g_pSprite->Begin(NULL);

		Sprite[0].Draw();
		Sprite[1].Draw();

		if (!flag) {
			if (!Sprite[1].addAlpha(-5)) flag = true;
		}
		else {
			//if (og->getState() != Dix::PCMPlayer::STATE_PLAY) og->Play(true);
		}

		inputDevice->getPushState();
		if (inputDevice->getPushStateAny(1)) g_FrameNo = SONG_SELECT_INIT;

		g_pSprite->End();
	}
}