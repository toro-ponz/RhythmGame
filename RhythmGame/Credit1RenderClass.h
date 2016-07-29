#pragma once

#include "SpriteDrawingClass.h"
#include "WavPlayClass.h"

#define CREDIT2_INIT         15
#define TITLE_INIT           20

using namespace DxSprite;
using namespace Wav;

namespace Frame {
	class Credit1Render {
	public:
		Credit1Render();
		~Credit1Render();
		void Rend(void);

	private:
		SpriteDrawing * Sprite;
		WavPlay * wv;
		int x;
		bool flag;
	};
}