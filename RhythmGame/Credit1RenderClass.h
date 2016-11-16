#pragma once

#include "SpriteDrawingClass.h"
#include "SoundPlayer.h"

using namespace DxSprite;
using namespace Sound;

namespace Frame {
	class Credit1Render {
	public:
		Credit1Render();
		~Credit1Render();
		void Rend(void);

	private:
		SpriteDrawing Logo;
		SpriteDrawing Black;
		DWORD oldTime;
		int state;
	};
}