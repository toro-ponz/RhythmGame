#pragma once

#include "SpriteDrawingClass.h"

#define SONG_SELECT_INIT         25

using namespace DxSprite;

namespace Frame {
	class ConfigRender {
	public:
		ConfigRender();
		~ConfigRender();
		void Rend(void);

	private:
		SpriteDrawing Title;
		SpriteDrawing Buttons;
	};
}