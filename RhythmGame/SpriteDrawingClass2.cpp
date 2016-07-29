#include "SpriteDrawingClass2.h"

namespace DxSprite {
	//コンストラクタ
	SpriteDrawing2::SpriteDrawing2(int x) {
		num = x;
		rec = new RECT[num];
		cen = new D3DXVECTOR3[num];
		pos = new D3DXVECTOR3[num];
		enable = new bool[num];
		for (int i = 0; i < num; i++) {
			setCenter(i);
			setPosition(i);
			enable[i] = false;
		}
	}

	//デストラクタ
	SpriteDrawing2::~SpriteDrawing2() {
		if (tex != NULL) {
			tex->Release();
			tex = NULL;
		}
		delete[] rec;
		delete[] cen;
		delete[] pos;
		delete[] enable;
	}

	//setTexture関数
	//引数 1　　　　　　　　　　　　　　　　　　画像パス
	//戻り値　　　　　　　　　　　　　　　　　　HRESULT
	HRESULT SpriteDrawing2::setTexture(LPCWSTR path) {
		HRESULT hr = D3DXCreateTextureFromFile(gl_lpD3ddev, path, &tex);
		// テクスチャサイズの取得
		D3DSURFACE_DESC desc;
		if (FAILED(tex->GetLevelDesc(0, &desc))) return hr;
		for (int i = 0; i < num; i++) {
			setRect(i, desc.Width, desc.Height);
		}
		return hr;
	}

	//setTexture関数
	//引数 1　　　　　　　　　　　　　　　　　　画像パス
	//戻り値　　　　　　　　　　　　　　　　　　HRESULT
	void SpriteDrawing2::setTexture(LPDIRECT3DTEXTURE9 pTex) {
		tex = pTex;
		// テクスチャサイズの取得
		D3DSURFACE_DESC desc;
		if (FAILED(tex->GetLevelDesc(0, &desc))) return;

		for (int i = 0; i < num; i++) {
			setRect(i, desc.Width, desc.Height);
		}
	}

	//setRect関数
	void SpriteDrawing2::setRect(int itt, LONG right, LONG bottom, LONG left, LONG top) {
		rec[itt].top = top;
		rec[itt].bottom = bottom;
		rec[itt].left = left;
		rec[itt].right = right;
	}

	//setCenter関数　　　　　　　　　　　　　　
	//引数 1
	//引数 2
	//戻り値
	void SpriteDrawing2::setCenter(int itt, float x, float y, float z) {
		cen[itt].x = x;
		cen[itt].y = y;
		cen[itt].z = z;
	}

	//setPosition関数　　　　　　　　　　　　　テクスチャの描画位置を指定
	//引数 1　　　　　　　　　　　　　　　　　　描画位置x
	//引数 2　　　　　　　　　　　　　　　　　　描画位置y
	//引数 3　　　　　　　　　　　　　　　　　　描画位置z
	//戻り値　　　　　　　　　　　　　　　　　　なし
	void SpriteDrawing2::setPosition(int itt, float x, float y, float z) {
		pos[itt].x = x;
		pos[itt].y = y;
		pos[itt].z = z;
	}

	//setColor関数　　　　　　　　　　　　　　　テクスチャのARGBを指定
	//引数 1　　　　　　　　　　　　　　　　　　D3DCOLOR
	//戻り値　　　　　　　　　　　　　　　　　　なし
	void SpriteDrawing2::setColor(int itt, int a, int r, int g, int b) {
		argb[0] = a;
		argb[1] = r;
		argb[2] = g;
		argb[3] = b;
	}

	//addPositionX関数
	//引数 1
	//戻り値
	float SpriteDrawing2::addPositionX(int itt, float add) {
		pos[itt].x += add;
		return pos[itt].x;
	}

	//addPositionY関数
	//引数 1
	//戻り値
	float SpriteDrawing2::addPositionY(int itt, float add) {
		pos[itt].y += add;
		return pos[itt].y;
	}

	//addAlpha関数
	//引数 1
	//戻り値
	bool SpriteDrawing2::addAlpha(int itt, int alpha) {
		if ((argb[0] + alpha > 255) || (argb[0] + alpha < 0)) {
			return false;
		}
		argb[0] += alpha;
		return true;
	}

	//putDisable関数
	void SpriteDrawing2::putDisable(int x) {
		enable[x] = false;
	}

	//putEnable関数
	void SpriteDrawing2::putEnable(int x) {
		enable[x] = true;
	}

	//Draw関数　　　　　　　　　　　　　　　　　テクスチャを描画
	//引数　　　　　　　　　　　　　　　　　　　なし
	//戻り値　　　　　　　　　　　　　　　　　　なし
	void SpriteDrawing2::Draw() {
		for (int i = 0; i < num; i++) {
			if (enable[i]) {
				g_pSprite->Draw(tex, &rec[i], &cen[i], &pos[i], D3DCOLOR_ARGB(argb[0], argb[1], argb[2], argb[3]));
			}
		}
	}
}