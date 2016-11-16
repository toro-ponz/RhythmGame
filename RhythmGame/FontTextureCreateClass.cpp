#include "FontTextureCreateClass.h"
#include "GlobalVariable.h"
#include <wchar.h>

namespace Font {
	/**
	*  �R���X�g���N�^
	*/
	FontTextureCreate::FontTextureCreate() :
		fontTextures() {

	}

	/**
	*  �f�X�g���N�^
	*/
	FontTextureCreate::~FontTextureCreate() {
		if (pTex != NULL) {
			pTex->Release();
			pTex = NULL;
		}
	}

	/**
	*  �t�H���g�e�N�X�`�����쐬����֐�.
	*  @param letters<string> �`�悷�镶����
	:  @param x<float> �`��J�n�ʒu��X���W
	*  @param y<float> �`��J�n�ʒu��Y���W
	*  @param size<int> �t�H���g�T�C�Y
	*  @param weight<int> �t�H���g�̑���
	*  @param font<string> �t�H���g
	*  @param horizontal<bool> ���������ۂ�
	*/
	void FontTextureCreate::fontCreate(string letters, float x, float y, int size, int weight, string font, bool horizontal) {
		wchar_t *c = convertCharToWchar_t(letters.c_str());
		lettersLength = wcslen(c);
		fontTextures.reserve(lettersLength);
		textMetric.reserve(lettersLength);
		glyphMetrics.reserve(lettersLength);
		monochrome = vector<vector<BYTE>>(lettersLength);
		startPositionX = x;
		startPositionY = y;
		isHorizontal = horizontal;
		setFontStatus(size, weight, font);
		FontCreate(c);
	}

	/**
	*  �t�H���g�e�N�X�`�����쐬����֐�.
	*  @param letters<TCHAR> ������
	*/
	void FontTextureCreate::FontCreate(wchar_t *letters) {
		HFONT hFont = CreateFontIndirect(&logFont);
		if (hFont == NULL) {
			Direct3DDevice9->Release();
			Direct3D9->Release();
			return;
		}

		// �f�o�C�X�Ƀt�H���g���������Ȃ���GetGlyphOutline�֐��̓G���[�ƂȂ�
		HDC hdc = GetDC(NULL);
		HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

		const int gradFlag = GGO_GRAY8_BITMAP; // GGO_GRAY2_BITMAP or GGO_GRAY4_BITMAP or GGO_GRAY8_BITMAP
		int grad = 0; // �K���̍ő�l
		switch (gradFlag) {
		case GGO_GRAY2_BITMAP: grad = 4; break;
		case GGO_GRAY4_BITMAP: grad = 16; break;
		case GGO_GRAY8_BITMAP: grad = 64; break;
		}
		if (grad == 0) {
			Direct3DDevice9->Release(); Direct3D9->Release();
			return;
		}
		
		for (int i = 0; i < lettersLength; i++) {
			wchar_t *letter = &letters[i];
			// �t�H���g�r�b�g�}�b�v�擾
			UINT code = (UINT)*letter;

			TEXTMETRIC tm;
			GetTextMetrics(hdc, &tm);
			GLYPHMETRICS gm;
			CONST MAT2 mat = { { 0,1 },{ 0,0 },{ 0,0 },{ 0,1 } };
			DWORD size = GetGlyphOutlineW(hdc, code, gradFlag, &gm, 0, NULL, &mat);
			BYTE *pMono = new BYTE[size];
			GetGlyphOutlineW(hdc, code, gradFlag, &gm, size, pMono, &mat);

			textMetric.push_back(tm);
			glyphMetrics.push_back(gm);
			for (int j = 0; j < (int)size; j++) {
				monochrome[i].push_back(pMono[j]);
			}

			if (maxGmBlackBoxX < gm.gmBlackBoxX)
				maxGmBlackBoxX = gm.gmBlackBoxX;
			if (maxGmBlackBoxY < gm.gmBlackBoxY)
				maxGmBlackBoxY = gm.gmBlackBoxY;
		}
		
		// �f�o�C�X�R���e�L�X�g�ƃt�H���g�n���h���͂�������Ȃ��̂ŉ��
		SelectObject(hdc, oldFont);
		ReleaseDC(NULL, hdc);

		// �e�N�X�`���쐬
		//IDirect3DTexture9 *pTex = 0;
		int textureWidth = ((maxGmBlackBoxX + 3) / 4 * 4);
		int textureHeight = maxGmBlackBoxY  * lettersLength;
		Direct3DDevice9->CreateTexture(textureWidth, textureHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTex, NULL);

		// �e�N�X�`���Ƀt�H���g�r�b�g�}�b�v������������
		D3DLOCKED_RECT lockedRect;
		pTex->LockRect(0, &lockedRect, NULL, 0);
		DWORD *pTexBuf = (DWORD*)lockedRect.pBits;   // �e�N�X�`���������ւ̃|�C���^

		for (int i = 0; i < lettersLength; i++) {
			int fontHeight = glyphMetrics[i].gmBlackBoxY;
			for (int y = i * maxGmBlackBoxY; y < (int)(i * maxGmBlackBoxY + fontHeight); y++) {
				int fontWidth = (glyphMetrics[i].gmBlackBoxX + 3) / 4 * 4;
				for (int x = 0; x < fontWidth; x++) {
					UINT monoSuffix = (UINT)((y - (i * maxGmBlackBoxY)) * fontWidth + x);
					UINT bufSuffix = (UINT)(y * textureWidth + x);
					DWORD alpha = monochrome[i][monoSuffix] * 255 / grad;
					pTexBuf[bufSuffix] = (alpha << 24) | 0x00ffffff;
				}
			}
		}
		pTex->UnlockRect(0);

		fontTextures.setTexture(pTex);
		fontTextures.setChipPixel((float)textureWidth, (float)maxGmBlackBoxY);
		setPosition(startPositionX, startPositionY);
	}

	/**
	*  �`�悷��֐�.
	*/
	void FontTextureCreate::Rend() {
		fontTextures.Draw();
	}

	/**
	*  �t�H���g�����쐬����֐�.
	*  @param size<int> �t�H���g�T�C�Y
	*  @param weight<int> �t�H���g�̑���
	*  @param font<TCHAR> �t�H���g
	*/
	void FontTextureCreate::setFontStatus(int size, int weight, string font) {
		fontSize = size;
		fontWeight = weight;

		logFont = {
			fontSize,
			0,
			0,
			0,
			fontWeight,
			0,
			0,
			0,
			SHIFTJIS_CHARSET,
			OUT_TT_ONLY_PRECIS,
			CLIP_DEFAULT_PRECIS,
			PROOF_QUALITY,
			DEFAULT_PITCH | FF_MODERN,
			*font.c_str()
		};
	}	

	/**
	*  �t�H���g�����쐬����֐�.
	*  @param x<float> �����X���W
	*  @param y<float> �����Y���W
	*/
	void FontTextureCreate::setPosition(float x, float y) {
		float startPositionX = x;
		float startPositionY = y;
		float posX = x;
		float posY = y;
		for (int i = 0; i < lettersLength; i++) {
			fontTextures.setRectFromChip(i, i + 1);
			fontTextures.setCenter(i, 0.0f, 0.0f, 0.0f);
			if (isHorizontal) {
				fontTextures.setPosition(i, posX + glyphMetrics[i].gmptGlyphOrigin.x, posY + textMetric[i].tmAscent - glyphMetrics[i].gmptGlyphOrigin.y);
				posX += glyphMetrics[i].gmCellIncX;
			}
			else {
				fontTextures.setPosition(i, posX + ((maxGmBlackBoxX - glyphMetrics[i].gmBlackBoxX) / 2), posY + 1.0f);
				posY += glyphMetrics[i].gmBlackBoxY;
			}
		}
	}

	/**
	*  �`���OFF�ɂ���֐�.
	*/
	void FontTextureCreate::disableDraw() {
		for (int i = 0; i < lettersLength; i++) {
			fontTextures.disableDraw(i);
		}
	}

	/**
	*  �`���ON�ɂ���֐�.
	*/
	void FontTextureCreate::enableDraw() {
		for (int i = 0; i < lettersLength; i++) {
			fontTextures.enableDraw(i);
		}
	}

	/**
	*  �����F��ݒ肷��֐�.
	*  @param c<D3DCOLOR> �����F(ARGB)
	*/
	void FontTextureCreate::setColor(D3DCOLOR c) {
		for (int i = 0; i < lettersLength; i++) {
			fontTextures.setColor(i, c);
		}
	}
}