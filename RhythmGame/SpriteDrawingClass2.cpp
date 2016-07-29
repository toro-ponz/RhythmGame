#include "SpriteDrawingClass2.h"

namespace DxSprite {
	//�R���X�g���N�^
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

	//�f�X�g���N�^
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

	//setTexture�֐�
	//���� 1�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�摜�p�X
	//�߂�l�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@HRESULT
	HRESULT SpriteDrawing2::setTexture(LPCWSTR path) {
		HRESULT hr = D3DXCreateTextureFromFile(gl_lpD3ddev, path, &tex);
		// �e�N�X�`���T�C�Y�̎擾
		D3DSURFACE_DESC desc;
		if (FAILED(tex->GetLevelDesc(0, &desc))) return hr;
		for (int i = 0; i < num; i++) {
			setRect(i, desc.Width, desc.Height);
		}
		return hr;
	}

	//setTexture�֐�
	//���� 1�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�摜�p�X
	//�߂�l�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@HRESULT
	void SpriteDrawing2::setTexture(LPDIRECT3DTEXTURE9 pTex) {
		tex = pTex;
		// �e�N�X�`���T�C�Y�̎擾
		D3DSURFACE_DESC desc;
		if (FAILED(tex->GetLevelDesc(0, &desc))) return;

		for (int i = 0; i < num; i++) {
			setRect(i, desc.Width, desc.Height);
		}
	}

	//setRect�֐�
	void SpriteDrawing2::setRect(int itt, LONG right, LONG bottom, LONG left, LONG top) {
		rec[itt].top = top;
		rec[itt].bottom = bottom;
		rec[itt].left = left;
		rec[itt].right = right;
	}

	//setCenter�֐��@�@�@�@�@�@�@�@�@�@�@�@�@�@
	//���� 1
	//���� 2
	//�߂�l
	void SpriteDrawing2::setCenter(int itt, float x, float y, float z) {
		cen[itt].x = x;
		cen[itt].y = y;
		cen[itt].z = z;
	}

	//setPosition�֐��@�@�@�@�@�@�@�@�@�@�@�@�@�e�N�X�`���̕`��ʒu���w��
	//���� 1�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�`��ʒux
	//���� 2�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�`��ʒuy
	//���� 3�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�`��ʒuz
	//�߂�l�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�Ȃ�
	void SpriteDrawing2::setPosition(int itt, float x, float y, float z) {
		pos[itt].x = x;
		pos[itt].y = y;
		pos[itt].z = z;
	}

	//setColor�֐��@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�e�N�X�`����ARGB���w��
	//���� 1�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@D3DCOLOR
	//�߂�l�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�Ȃ�
	void SpriteDrawing2::setColor(int itt, int a, int r, int g, int b) {
		argb[0] = a;
		argb[1] = r;
		argb[2] = g;
		argb[3] = b;
	}

	//addPositionX�֐�
	//���� 1
	//�߂�l
	float SpriteDrawing2::addPositionX(int itt, float add) {
		pos[itt].x += add;
		return pos[itt].x;
	}

	//addPositionY�֐�
	//���� 1
	//�߂�l
	float SpriteDrawing2::addPositionY(int itt, float add) {
		pos[itt].y += add;
		return pos[itt].y;
	}

	//addAlpha�֐�
	//���� 1
	//�߂�l
	bool SpriteDrawing2::addAlpha(int itt, int alpha) {
		if ((argb[0] + alpha > 255) || (argb[0] + alpha < 0)) {
			return false;
		}
		argb[0] += alpha;
		return true;
	}

	//putDisable�֐�
	void SpriteDrawing2::putDisable(int x) {
		enable[x] = false;
	}

	//putEnable�֐�
	void SpriteDrawing2::putEnable(int x) {
		enable[x] = true;
	}

	//Draw�֐��@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�e�N�X�`����`��
	//�����@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�Ȃ�
	//�߂�l�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�Ȃ�
	void SpriteDrawing2::Draw() {
		for (int i = 0; i < num; i++) {
			if (enable[i]) {
				g_pSprite->Draw(tex, &rec[i], &cen[i], &pos[i], D3DCOLOR_ARGB(argb[0], argb[1], argb[2], argb[3]));
			}
		}
	}
}