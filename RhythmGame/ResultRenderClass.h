#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "SpriteDrawingClass.h"
#include "InputDevice.h"

using namespace DxSprite;

namespace Frame {
	class ResultRender {
	public:
		ResultRender(int, int, int, int, int, int, int);
		~ResultRender();
		void Rend();

	private:
		SpriteDrawing Result;
		int result[7];
		int count[7] = { };
		int countUp;

	};
}