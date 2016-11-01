#pragma once
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>

extern LPDIRECT3DDEVICE9 Direct3DDevice9;
extern LPD3DXSPRITE Sprite;

using namespace std;

namespace DxSprite {
	class SpriteDrawing {
	public:
		SpriteDrawing(int = 1, float = 32.0f);
		SpriteDrawing(TCHAR*, int = 1, float = 32.0f);
		~SpriteDrawing();
		HRESULT setTexture(TCHAR*);
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
		void reserve(int);
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