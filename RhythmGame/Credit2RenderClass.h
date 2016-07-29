#pragma once

#include "SpriteDrawingClass.h"
#include "WavPlayClass.h"

#define TITLE_INIT           20

using namespace DxSprite;
using namespace Wav;

namespace Frame {
	class Credit2Render {
	public:
		Credit2Render();
		~Credit2Render();
		void Rend(void);

	private:
		SpriteDrawing * Sprite;
		WavPlay * wv;
		int x;
		bool flag;
	};
}