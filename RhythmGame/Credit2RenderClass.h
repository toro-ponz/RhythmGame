#pragma once

#include "SpriteDrawingClass.h"
#include "SoundPlayer.h"

#define TITLE_INIT           20

using namespace DxSprite;
using namespace Sound;

namespace Frame {
	class Credit2Render {
	public:
		Credit2Render();
		~Credit2Render();
		void Rend(void);

	private:
		SpriteDrawing Logo;
		SpriteDrawing Black;
		WavPlayer * wv;
		int x;
		bool flag;
	};
}