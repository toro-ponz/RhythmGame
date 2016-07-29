#pragma once
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>

extern LPDIRECT3DDEVICE9 gl_lpD3ddev;
extern LPD3DXSPRITE g_pSprite;

namespace DxSprite {
	class SpriteDrawing2 {
	public:
		SpriteDrawing2(int);
		~SpriteDrawing2();
		HRESULT setTexture(LPCWSTR);
		void setTexture(LPDIRECT3DTEXTURE9);
		void setRect(int, LONG, LONG, LONG left = 0, LONG top = 0);
		void setCenter(int, float x = 0, float y = 0, float z = 0);
		void setPosition(int, float x = 0, float y = 0, float z = 0);
		void setColor(int, int, int, int, int);
		float addPositionX(int, float);
		float addPositionY(int, float);
		bool addAlpha(int, int);
		void putDisable(int);
		void putEnable(int);
		void Draw();

	private:
		LPDIRECT3DTEXTURE9 tex;
		RECT * rec;
		D3DXVECTOR3 * cen;
		D3DXVECTOR3 * pos;
		int argb[4] = {255, 255, 255, 255};
		bool * enable;
		int num = 0;

	};
}