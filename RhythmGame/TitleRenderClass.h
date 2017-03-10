#pragma once

#include "SpriteDrawingClass.h"
#include "SoundPlayer.h"

using namespace DxSprite;
using namespace Sound;

namespace Frame {
	class TitleRender {
	public:
		TitleRender();
		~TitleRender();
		void Rend();

	private:
		SpriteDrawing Background;
		SpriteDrawing Title;
		SpriteDrawing Black;
		DWORD oldTime;
	};
}