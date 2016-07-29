#pragma once

#include "SpriteDrawingClass.h"
#include "FontTextureCreateClass.h"

#define PLAY_EASY_INIT       30
#define PLAY_NORMAL_INIT     35
#define PLAY_HARD_INIT       40
#define CONFIG_INIT          55

using namespace DxSprite;
using namespace Font;

namespace Frame {
	class SongSelectRender {
	public:
		SongSelectRender();
		~SongSelectRender();
		void Rend(void);

	private:
		SpriteDrawing * Sprite;
		FontTextureCreate * ftc1;
		int sNum = 1;//ã»î‘çÜ
		int SCE = 0;//ï™äÚ
		int level = 1;
	};
}