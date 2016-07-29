#pragma once
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>
#include "SpriteDrawingClass.h"

using namespace DxSprite;

namespace Font {
	class FontTextureCreate {
	public:
		FontTextureCreate(TCHAR*, int, int, TCHAR*, float, float, bool);
		~FontTextureCreate();
		void Rend(void);
		void FontCreate(TCHAR*, int);
		void setFontStatus(int, int, TCHAR);

	protected:

	private:
		SpriteDrawing * Sprite;
		LOGFONT lf;
		LPDIRECT3DTEXTURE9 pTex;
		int fontSize;
		int fontWeight;
		int len;
		float tx, ty;

		struct Vtx {
			float x, y, z;
			float u, v;
		};
	};
}