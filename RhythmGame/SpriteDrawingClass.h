#pragma once
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>

extern LPDIRECT3DDEVICE9 gl_lpD3ddev;
extern LPD3DXSPRITE g_pSprite;

namespace DxSprite {
	class SpriteDrawing {
	public:
		SpriteDrawing();
		~SpriteDrawing();
		HRESULT setTexture(LPCWSTR);
		void setTexture(LPDIRECT3DTEXTURE9);
		void setRect(LONG, LONG);
		void setCenter(float x = 0, float y = 0, float z = 0);
		void setPosition(float x = 0, float y = 0, float z = 0);
		void setColor(int, int, int, int);
		float addPositionX(float);
		float addPositionY(float);
		bool addAlpha(int);
		void Draw();

	private:
		LPDIRECT3DTEXTURE9 tex;
		RECT rec;
		D3DXVECTOR3 cen;
		D3DXVECTOR3 pos;
		int argb[4] = {255, 255, 255, 255};

	};
}