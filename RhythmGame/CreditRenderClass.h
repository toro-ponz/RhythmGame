#pragma once

#include "SpriteDrawingClass.h"
#include "SoundPlayer.h"

using namespace DxSprite;
using namespace Sound;

namespace Frame {
	class CreditRender {
	public:
		CreditRender();
		~CreditRender();
		void Rend(void);

	private:
		SpriteDrawing Logo;
		SpriteDrawing Black;
		DWORD oldTime;
		int state;
	};
}