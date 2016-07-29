#pragma once
#include <string>
#include "SpriteDrawingClass3.h"
#include "InputDeviceClass.h"

using namespace DxSprite;

namespace Frame {
	class ResultRender {
	public:
		ResultRender(int, int, int, int, int, int, int);
		~ResultRender();
		void Rend();

	private:
		SpriteDrawing3 * Sprite;
		int result[7];
		int count[7] = { };
		int countUp;

	};
}