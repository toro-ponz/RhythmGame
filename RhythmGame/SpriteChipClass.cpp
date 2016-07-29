#include "SpriteChipClass.h"

namespace Sprite {
	//コンストラクタ
	//引数 1                                    要素数
	//戻値                                      なし
	SpriteChip::SpriteChip(int x, float tile) :
			num					(x),
			tilePixel     		(tile) {
		rec.reserve(num);
		cen.reserve(num);
		pos.reserve(num);
		color.reserve(num);
		draw.reserve(num);
		for (int i = 0; i < num; i++) {
			rec.push_back({ 0, 0, 0, 0 });
			cen.push_back({ 0.0f, 0.0f, 0.0f });
			pos.push_back({ 0.0f, 0.0f, 0.0f });
			color.push_back(D3DCOLOR_ARGB(255, 255, 255, 255));
			draw.push_back(true);
		}
	}

	//コンストラクタ
	//引数 1                                    画像パス
	//引数 2                                    要素数
	//戻値                                      なし
	SpriteChip::SpriteChip(LPCWSTR tex, int x, float tile) :
			num					(x),
			tilePixel			(tile) {
		rec.reserve(num);
		cen.reserve(num);
		pos.reserve(num);
		draw.reserve(num);
		for (int i = 0; i < num; i++) {
			rec.push_back({ 0, 0, 0, 0 });
			cen.push_back({ 0.0f, 0.0f, 0.0f });
			pos.push_back({ 0.0f, 0.0f, 0.0f });
			color.push_back(D3DCOLOR_ARGB(255, 255, 255, 255));
			draw.push_back(true);
		}
		setTexture(tex);
	}

	//デストラクタ
	//引数                                      なし
	//戻値                                      なし
	SpriteChip::~SpriteChip() {
		if (tex != NULL) {
			tex->Release();
			tex = NULL;
		}
	}

	//setTexture関数                            テクスチャをセットする
	//引数 1                                    画像パス
	//戻値                                      HRESULT
	HRESULT SpriteChip::setTexture(LPCWSTR path) {
		HRESULT hr = D3DXCreateTextureFromFile(gl_lpD3ddev, path, &tex);
		// テクスチャサイズの取得
		D3DSURFACE_DESC desc;
		if (FAILED(tex->GetLevelDesc(0, &desc))) return hr;

		texWidth = desc.Width;
		texHeight = desc.Height;
		for (int i = 0; i < num; i++) {
			setRect(i, texWidth, texHeight);
		}
		return hr;
	}

	//setTexture関数                            テクスチャをセットする
	//引数 1                                    テクスチャのポインタ
	//戻値                                      なし
	void SpriteChip::setTexture(LPDIRECT3DTEXTURE9 pTex) {
		tex = pTex;
		// テクスチャサイズの取得
		D3DSURFACE_DESC desc;
		if (FAILED(tex->GetLevelDesc(0, &desc))) return;

		texWidth = desc.Width;
		texHeight = desc.Height;
		for (int i = 0; i < num; i++) {
			setRect(i, texWidth, texHeight);
		}
	}

	//setRect関数                               RECT構造体をセットする
	//引数 1                                    添え字
	//引数 2                                    RECT.right
	//引数 3                                    RECT.bottom
	//引数 4                                    RECT.left
	//引数 5                                    RECT.top
	//戻値                                      なし
	void SpriteChip::setRect(int sub, LONG right, LONG bottom, LONG left, LONG top) {
		rec[sub].top =  top;
		rec[sub].bottom = bottom;
		rec[sub].left = left;
		rec[sub].right =  right;
	}

	//setRectC関数                              RECT構造体をセットする
	//引数 1                                    添え字
	//引数 2                                    画像チップの開始インデックス値
	//引数 3                                    画像チップの終了インデックス値
	//戻値                                      なし
	void SpriteChip::setRectC(int sub, int start, int end) {
		int wTile = (int)(texWidth / tilePixel);
		float left = tilePixel * (start - (wTile * (int)((start - 1) / wTile)) - 1);
		float top = tilePixel * (int)((start - 1) / wTile);
		float right = tilePixel * (end - (wTile * (int)((end - 1) / wTile)));
		float bottom = tilePixel * (int)((end - 1) / wTile + 1);
		setRect(sub, (UINT)right, (UINT)bottom, (UINT)left, (UINT)top);
	}

	//setCenter関数                             テクスチチャのCenterをセットする
	//引数 1                                    添え字
	//引数 2                                    Center.x
	//引数 3                                    Center.y
	//引数 4                                    Center.z
	//戻値                                      なし
	void SpriteChip::setCenter(int sub, float x, float y, float z) {
		cen[sub].x = x;
		cen[sub].y = y;
		cen[sub].z = z;
	}

	//setPosition関数                           テクスチャの描画位置を指定
	//引数 1                                    Position.x
	//引数 2                                    Position.y
	//引数 3                                    Position.z
	//戻値                                      なし
	void SpriteChip::setPosition(int sub, float x, float y, float z) {
		pos[sub].x = x;
		pos[sub].y = y;
		pos[sub].z = z;
	}

	//setColor関数                              テクスチャのARGBを指定
	//引数 1                                    D3DCOLOR_ARGB
	//引数 2                                    添え字
	//戻値                                      なし
	void SpriteChip::setColor(D3DCOLOR c, int sub) {
		color[sub] = c;
	}

	//addPositionX関数                          描画位置をずらす
	//引数 1                                    X座標の増減
	//引数 2                                    添え字
	//戻値                                      計算後の描画位置のX座標
	float SpriteChip::addPositionX(float add, int sub) {
		pos[sub].x += add;
		return pos[sub].x;
	}

	//addPositionY関数                          描画位置をずらす
	//引数 1                                    Y座標の増減
	//引数 2                                    添え字
	//戻値                                      計算後の描画位置のX座標
	float SpriteChip::addPositionY(float add, int sub) {
		pos[sub].y += add;
		return pos[sub].y;
	}

	//addAlpha関数                              透明度を変える
	//引数 1                                    不透明度の増減
	//引数 2                                    添え字
	//戻値                                      不透明度の範囲(0~255)を超えた場合falseを返す
	bool SpriteChip::addAlpha(int alpha, int sub) {
		unsigned int a = ((color[sub] & 0xff000000) >> 24) + alpha;
		if (a > 256) return false;
		color[sub] = (a << 24) | (color[sub] & 0x00ffffff);
		return true;
	}

	//disableDraw関数                           描画をOFFにする
	//引数 1                                    添え字
	//戻値                                      なし
	void SpriteChip::disableDraw(int sub) {
		draw[sub] = false;
	}

	//enableDraw関数                            描画をONにする
	//引数 1                                    添え字
	//戻値                                      なし
	void SpriteChip::enableDraw(int sub) {
		draw[sub] = true;
	}

	//setTile関数                               画像チップの1つの幅をセットする
	//引数 1                                    チップ幅
	//戻値                                      なし
	void SpriteChip::setTile(float t) {
		tilePixel = t;
	}

	//Draw関数                                  テクスチャを描画する
	//引数                                      なし
	//戻値                                      なし
	void SpriteChip::Draw() {
		for (int i = 0; i < num; i++) {
			if (draw[i]) {
				g_pSprite->Draw(tex, &rec[i], &cen[i], &pos[i], color[i]);
			}
		}
	}
}