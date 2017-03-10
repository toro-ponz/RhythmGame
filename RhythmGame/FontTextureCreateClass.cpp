#include "FontTextureCreateClass.h"
#include "Global.h"
#include <wchar.h>

namespace Font {
	/**
	*  �R���X�g���N�^
	*/
	FontTextureCreate::FontTextureCreate() :
			fontTextures() {
		maxHeight = 0.0f;
		maxWidth = 0.0f;
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
	*  @param l<string> �`�悷�镶����
	*  @param x<float> �`��J�n�ʒu��X���W
	*  @param y<float> �`��J�n�ʒu��Y���W
	*  @param size<int> �t�H���g�T�C�Y
	*  @param weight<int> �t�H���g�̑���
	*  @param font<string> �t�H���g
	*  @param horizontal<bool> ���������ۂ�
	*/
	void FontTextureCreate::fontCreate(string l, float x, float y, int size, int weight, string font, bool horizontal) {
		letters = l;

		maxWidth = 0.0f;
		maxHeight = 0.0f;
		textMetric.clear();
		glyphMetrics.clear();
		monochrome.clear();
		textMetric.shrink_to_fit();
		glyphMetrics.shrink_to_fit();
		monochrome.shrink_to_fit();
		maxGmBlackBoxX = 0;
		maxGmBlackBoxY = 0;
		pTex = NULL;

		wchar_t *lettersWchar = convertCharToWchar_t(letters.c_str());
		lettersLength = wcslen(lettersWchar);
		fontTextures.reserve(lettersLength);
		textMetric.reserve(lettersLength);
		glyphMetrics.reserve(lettersLength);
		monochrome = vector<vector<BYTE>>(lettersLength);
		startPositionX = x;
		startPositionY = y;
		isHorizontal = horizontal;
		setFontStatus(size, weight, font);
		
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
			wchar_t *letter = &lettersWchar[i];
			// �t�H���g�r�b�g�}�b�v�擾
			UINT code = (UINT)*letter;

			TEXTMETRIC tm;
			GetTextMetrics(hdc, &tm);
			GLYPHMETRICS gm;
			CONST MAT2 mat = { { 0,1 },{ 0,0 },{ 0,0 },{ 0,1 } };
			DWORD size = GetGlyphOutlineW(hdc, code, gradFlag, &gm, 0, NULL, &mat);
			BYTE *pMono = new BYTE[size];


			if (GetGlyphOutlineW(hdc, code, gradFlag, &gm, size, pMono, &mat) != GDI_ERROR) {
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
			wchar_t letter[2];
			letter[0] = lettersWchar[i];
			//�I�[����
			letter[1] = L'\x0';
			if (!wcscmp(letter, L"�@") || !wcscmp(letter, L" ") || !wcscmp(letter, L"\n")) {
				for (int y = i * maxGmBlackBoxY; y < (int)(i * maxGmBlackBoxY + fontHeight); y++) {
					int fontWidth = (glyphMetrics[i].gmBlackBoxX + 3) / 4 * 4;
					for (int x = 0; x < fontWidth; x++) {
						UINT monoSuffix = (UINT)((y - (i * maxGmBlackBoxY)) * fontWidth + x);
						UINT bufSuffix = (UINT)(y * textureWidth + x);
						DWORD alpha = 0 * 255 / grad;
						pTexBuf[bufSuffix] = (alpha << 24) | 0x00ffffff;
					}
				}
			}
			else {
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
		}
		pTex->UnlockRect(0);

		fontTextures.setTexture(pTex);
		fontTextures.setChipPixel((float)textureWidth, (float)maxGmBlackBoxY);
		setPosition(startPositionX, startPositionY);

		for (int i = 0; i < lettersLength; i++) {
			fontTextures.enableDraw(i);
		}
		for (int i = lettersLength; i < fontTextures.getElementCount(); i++) {
			fontTextures.disableDraw(i);
		}
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
			*convertStringToTchar(font)
		};
	}

	/**
	*  �e�����̕\���ʒu���v�Z����֐�.
	*  @param x<float> �����X���W
	*  @param y<float> �����Y���W
	*/
	void FontTextureCreate::setPosition(float x, float y) {
		wchar_t *lettersWchar = convertCharToWchar_t(letters.c_str());
		float startPositionX = x;
		float startPositionY = y;
		float posX = x;
		float posY = y;
		for (int i = 0; i < lettersLength; i++) {
			fontTextures.setRectFromChip(i, i + 1);
			fontTextures.setCenter(i, 0.0f, 0.0f, 0.0f);
			wchar_t letter[2];
			letter[0] = lettersWchar[i];
			//�I�[����
			letter[1] = L'\x0';
			if (!wcscmp(letter, L"\n")){
				if (isHorizontal) {
					posX = startPositionX;
					posY += maxGmBlackBoxY;
				}
				else {
					posX += maxGmBlackBoxX;
					posY = startPositionY;
				}
			}
			else {
				if (isHorizontal) {
					if (maxWidth != 0.0f && posX + glyphMetrics[i].gmCellIncX > maxWidth + startPositionX) {
						posX = startPositionX;
						posY += maxGmBlackBoxY;
					}
					fontTextures.setPosition(i, posX + glyphMetrics[i].gmptGlyphOrigin.x, posY + textMetric[i].tmAscent - glyphMetrics[i].gmptGlyphOrigin.y);
					posX += glyphMetrics[i].gmCellIncX;
				}
				else {
					if (maxHeight != 0.0f && posY + glyphMetrics[i].gmBlackBoxY > maxHeight + startPositionY) {
						posX += maxGmBlackBoxX;
						posY = startPositionY;
					}
					fontTextures.setPosition(i, posX + ((maxGmBlackBoxX - glyphMetrics[i].gmBlackBoxX) / 2), posY);
					posY += glyphMetrics[i].gmBlackBoxY;
				}
			}
		}
	}

	/**
	*  ������̐܂�Ԃ����܂��͍�����ݒ肷��֐�.
	*  @param px<int> ������̐܂�Ԃ����܂��͍���
	*/
	void FontTextureCreate::setWidthOrHeight(float px) {
		if (isHorizontal)
			maxWidth = px;
		else
			maxHeight = px;
		setPosition(startPositionX, startPositionY);
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