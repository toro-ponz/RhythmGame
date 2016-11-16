#include "ConfigRenderClass.h"
#include "InputDevice.h"
#include "GlobalVariable.h"

namespace Frame {
	/**
	*  コンストラクタ
	*/
	ConfigRender::ConfigRender() :
			Title(configImageDirectoryPath + "title.png"),
			Buttons(configImageDirectoryPath + "buttons.png") {
		//Sprite[0].setPosition(0.0, 0.0);
		//Sprite[1].setPosition(512.0, -70.0);
		//Sprite[2].setPosition(768.0, -70.0);
		//Sprite[3].setPosition(845.0, 256.0);
		//Sprite[4].setPosition(845.0, 512.0);
	}

	/**
	*  デストラクタ
	*/
	ConfigRender::~ConfigRender() {

	}

	/**
	*  描画する関数.
	*/
	void ConfigRender::Rend(void) {
		Sprite->Begin(NULL);

		//Sprite[0].Draw();
		//Sprite[1].Draw();
		//Sprite[2].Draw();
		//Sprite[3].Draw();
		//Sprite[4].Draw();

		inputDevice->getPushState();
		if (inputDevice->getPushStateAny(InputDevice::KEY_STATE::STATE_PUSH))
			frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::SONG_SELECT_INIT);

		Sprite->End();
	}
}