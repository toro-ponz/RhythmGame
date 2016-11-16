#pragma once

#include "SpriteDrawingClass.h"

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