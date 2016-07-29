#include "FontTextureCreateClass.h"
#include <wchar.h>

extern LPDIRECT3DDEVICE9 gl_lpD3ddev;
extern LPDIRECT3D9 gl_lpD3d;
extern LPD3DXSPRITE g_pSprite;

namespace Font {
	//コンストラクタ
	FontTextureCreate::FontTextureCreate(TCHAR * c, int Size, int Weight, TCHAR * font, float x, float y, bool b) {
		len = wcslen(c);
		tx = x;
		ty = y;
		Sprite = new SpriteDrawing[len];
		wchar_t s[100];
		wcscpy_s(s, c);
		setFontStatus(Size, Weight, *font);
		for (int i = 0; i < len; i++) {
			FontCreate(&s[i], i);
		}
	}

	//デストラクタ
	FontTextureCreate::~FontTextureCreate() {
		if (pTex != NULL) {
			pTex->Release();
			pTex = NULL;
		}
		delete[] Sprite;
	}

	//FontCreate関数　　　　　　　　　　　フォントテクスチャを作成する
	//引数 1　　　　　　　　　　　　　　　文字
	//戻り値　　　　　　　　　　　　　　　なし
	void FontTextureCreate::FontCreate(TCHAR * leter, int i) {
		HFONT hFont = CreateFontIndirect(&lf);
		if (hFont == NULL) {
			gl_lpD3ddev->Release(); gl_lpD3d->Release();
			return;
		}

		// デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
		HDC hdc = GetDC(NULL);
		HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

		// フォントビットマップ取得
		UINT code = (UINT)*leter;
		const int gradFlag = GGO_GRAY4_BITMAP; // GGO_GRAY2_BITMAP or GGO_GRAY4_BITMAP or GGO_GRAY8_BITMAP
		int grad = 0; // 階調の最大値
		switch (gradFlag) {
		case GGO_GRAY2_BITMAP: grad = 4; break;
		case GGO_GRAY4_BITMAP: grad = 16; break;
		case GGO_GRAY8_BITMAP: grad = 64; break;
		}
		if (grad == 0) {
			gl_lpD3ddev->Release(); gl_lpD3d->Release();
			return;
		}

		TEXTMETRIC tm;
		GetTextMetrics(hdc, &tm);
		GLYPHMETRICS gm;
		CONST MAT2 mat = { { 0,1 },{ 0,0 },{ 0,0 },{ 0,1 } };
		DWORD size = GetGlyphOutlineW(hdc, code, gradFlag, &gm, 0, NULL, &mat);
		BYTE *pMono = new BYTE[size];
		GetGlyphOutlineW(hdc, code, gradFlag, &gm, size, pMono, &mat);

		// デバイスコンテキストとフォントハンドルはもういらないので解放
		SelectObject(hdc, oldFont);
		ReleaseDC(NULL, hdc);

		// テクスチャ作成
		//IDirect3DTexture9 *pTex = 0;
		int fontWidth = (gm.gmBlackBoxX + 3) / 4 * 4;
		int fontHeight = gm.gmBlackBoxY;
		gl_lpD3ddev->CreateTexture(fontWidth, fontHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTex, NULL);

		// テクスチャにフォントビットマップ情報を書き込み
		D3DLOCKED_RECT lockedRect;
		pTex->LockRect(0, &lockedRect, NULL, 0);  // ロック
		DWORD *pTexBuf = (DWORD*)lockedRect.pBits;   // テクスチャメモリへのポインタ

		for (int y = 0; y < fontHeight; y++) {
			for (int x = 0; x < fontWidth; x++) {
				DWORD alpha = pMono[y * fontWidth + x] * 255 / grad;
				pTexBuf[y * fontWidth + x] = (alpha << 24) | 0x00ffffff;
			}
		}

		pTex->UnlockRect(0);  // アンロック
		delete[] pMono;
		Sprite[i].setTexture(pTex);
		Sprite[i].setRect(fontWidth, fontHeight);
		Sprite[i].setCenter(0.0f, 0.0f, 0.0f);
		Sprite[i].setPosition(tx + gm.gmptGlyphOrigin.x , ty + (tm.tmAscent - gm.gmptGlyphOrigin.y), 0.0f);

		//tx += fontWidth + 2.0f;
		tx += gm.gmCellIncX;
	}

	//Rend関数
	void FontTextureCreate::Rend(void) {
		for (int i = 0; i < len; i++) {
			Sprite[i].Draw();
		}
	}

	//setFontStatus関数 　　　　　　　　　フォント情報を与える
	//引数 1　　　　　　　　　　　　　　　フォントのサイズ
	//引数 2　　　　　　　　　　　　　　　フォントの太さ
	//引数 3　　　　　　　　　　　　　　　フォント
	//戻り値　　　　　　　　　　　　　　　なし
	void FontTextureCreate::setFontStatus(int Size, int Weight, TCHAR font) {
		fontSize = Size;
		fontWeight = Weight;

		lf = { fontSize, 0, 0, 0, fontWeight, 0, 0, 0, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN, _T("メイリオ") };
	}
}