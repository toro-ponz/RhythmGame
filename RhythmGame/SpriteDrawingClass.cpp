#include "SpriteDrawingClass.h"

namespace DxSprite {
	//コンストラクタ
	SpriteDrawing::SpriteDrawing() {
		setCenter();
		setPosition();
	}

	//デストラクタ
	SpriteDrawing::~SpriteDrawing() {
		if (tex != NULL) {
			tex->Release();
			tex = NULL;
		}
	}

	//setTexture関数
	//引数 1　　　　　　　　　　　　　　　　　　画像パス
	//戻り値　　　　　　　　　　　　　　　　　　HRESULT
	HRESULT SpriteDrawing::setTexture(LPCWSTR path) {
		HRESULT hr = D3DXCreateTextureFromFile(gl_lpD3ddev, path, &tex);
		// テクスチャサイズの取得
		D3DSURFACE_DESC desc;
		if (FAILED(tex->GetLevelDesc(0, &desc))) return hr;

		setRect(desc.Width, desc.Height);
		return hr;
	}

	//setTexture関数
	//引数 1　　　　　　　　　　　　　　　　　　画像パス
	//戻り値　　　　　　　　　　　　　　　　　　HRESULT
	void SpriteDrawing::setTexture(LPDIRECT3DTEXTURE9 pTex) {
		tex = pTex;
		// テクスチャサイズの取得
		D3DSURFACE_DESC desc;
		if (FAILED(tex->GetLevelDesc(0, &desc))) return;

		setRect(desc.Width, desc.Height);
	}

	//setRect関数
	//引数 1　　　　　　　　　　　　　　　　　　テクスチャの横の長さ
	//引数 2　　　　　　　　　　　　　　　　　　テクスチャの縦の長さ
	//戻り値　　　　　　　　　　　　　　　　　　なし
	void SpriteDrawing::setRect(LONG right, LONG bottom) {
		rec.top = 0;
		rec.bottom = bottom;
		rec.left = 0;
		rec.right = right;
	}

	//setCenter関数　　　　　　　　　　　　　　
	//引数 1
	//引数 2
	//戻り値
	void SpriteDrawing::setCenter(float x, float y, float z) {
		cen.x = x;
		cen.y = y;
		cen.z = z;
	}

	//setPosition関数　　　　　　　　　　　　　テクスチャの描画位置を指定
	//引数 1　　　　　　　　　　　　　　　　　　描画位置x
	//引数 2　　　　　　　　　　　　　　　　　　描画位置y
	//引数 3　　　　　　　　　　　　　　　　　　描画位置z
	//戻り値　　　　　　　　　　　　　　　　　　なし
	void SpriteDrawing::setPosition(float x, float y, float z) {
		pos.x = x;
		pos.y = y;
		pos.z = z;
	}

	//setColor関数　　　　　　　　　　　　　　　テクスチャのARGBを指定
	//引数 1　　　　　　　　　　　　　　　　　　D3DCOLOR
	//戻り値　　　　　　　　　　　　　　　　　　なし
	void SpriteDrawing::setColor(int a, int r, int g, int b) {
		argb[0] = a;
		argb[1] = r;
		argb[2] = g;
		argb[3] = b;
	}

	//addPositionX関数
	//引数 1
	//戻り値
	float SpriteDrawing::addPositionX(float add) {
		pos.x += add;
		return pos.x;
	}

	//addPositionY関数
	//引数 1
	//戻り値
	float SpriteDrawing::addPositionY(float add) {
		pos.y += add;
		return pos.y;
	}

	//addAlpha関数
	//引数 1
	//戻り値
	bool SpriteDrawing::addAlpha(int alpha) {
		if ((argb[0] + alpha > 255) || (argb[0] + alpha < 0)) {
			return false;
		}
		argb[0] += alpha;
		return true;
	}

	//Draw関数　　　　　　　　　　　　　　　　　テクスチャを描画
	//引数　　　　　　　　　　　　　　　　　　　なし
	//戻り値　　　　　　　　　　　　　　　　　　なし
	void SpriteDrawing::Draw() {
		g_pSprite->Draw(tex, &rec, &cen, &pos, D3DCOLOR_ARGB(argb[0], argb[1], argb[2], argb[3]));
	}
}