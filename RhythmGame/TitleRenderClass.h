#pragma once
#include "SpriteDrawingClass.h"
#include "SoundPlayer.h"

#define SONG_SELECT_INIT     25

using namespace DxSprite;
using namespace Sound;

namespace Frame {
	class TitleRender {
	public:
		TitleRender();
		~TitleRender();
		void Rend(void);

	private:
		SpriteDrawing Title;
		SpriteDrawing Black;
		OggPlayer * og;
		bool flag;

	};
}