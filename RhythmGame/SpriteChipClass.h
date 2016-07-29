#pragma once
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

extern LPDIRECT3DDEVICE9 gl_lpD3ddev;
extern LPD3DXSPRITE g_pSprite;

using namespace std;

namespace Sprite {
	class SpriteChip {
	public:
		SpriteChip(int = 1, float = 32.0f);
		SpriteChip(LPCWSTR, int = 1, float = 32.0f);
		~SpriteChip();
		HRESULT setTexture(LPCWSTR);
		void setTexture(LPDIRECT3DTEXTURE9);
		void setRect(int, LONG, LONG, LONG = 0, LONG = 0);
		void setRectC(int, int, int);
		void setCenter(int = 0, float = 0, float = 0, float = 0);
		void setPosition(int = 0, float = 0, float = 0, float = 0);
		void setColor(D3DCOLOR, int = 0);
		float addPositionX(float, int = 0);
		float addPositionY(float, int = 0);
		bool addAlpha(int, int = 0);
		void disableDraw(int = 0);
		void enableDraw(int = 0);
		void setTile(float);
		void Draw();

	private:
		LPDIRECT3DTEXTURE9 tex;
		vector<RECT> rec;
		vector<D3DXVECTOR3> cen;
		vector<D3DXVECTOR3> pos;
		vector<D3DCOLOR> color;
		vector<bool> draw;
		int num;
		unsigned int texWidth, texHeight;
		float tilePixel;
	};
}