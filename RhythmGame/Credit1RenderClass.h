#pragma once

#include "SpriteDrawingClass.h"
#include "SoundPlayer.h"

#define CREDIT2_INIT         15
#define TITLE_INIT           20

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
		WavPlayer * wv;
		int x;
		bool flag;
	};
}