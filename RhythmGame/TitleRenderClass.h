#pragma once
#include "SpriteDrawingClass.h"
#include "OggPlayClass.h"

#define SONG_SELECT_INIT     25

using namespace DxSprite;
using namespace Ogg;

namespace Frame {
	class TitleRender {
	public:
		TitleRender();
		~TitleRender();
		void Rend(void);

	private:
		SpriteDrawing * Sprite;
		OggPlay * og;
		bool flag;

	};
}