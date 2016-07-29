#include "SpriteDrawingClass.h"

namespace DxSprite {
	//�R���X�g���N�^
	SpriteDrawing::SpriteDrawing() {
		setCenter();
		setPosition();
	}

	//�f�X�g���N�^
	SpriteDrawing::~SpriteDrawing() {
		if (tex != NULL) {
			tex->Release();
			tex = NULL;
		}
	}

	//setTexture�֐�
	//���� 1�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�摜�p�X
	//�߂�l�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@HRESULT
	HRESULT SpriteDrawing::setTexture(LPCWSTR path) {
		HRESULT hr = D3DXCreateTextureFromFile(gl_lpD3ddev, path, &tex);
		// �e�N�X�`���T�C�Y�̎擾
		D3DSURFACE_DESC desc;
		if (FAILED(tex->GetLevelDesc(0, &desc))) return hr;

		setRect(desc.Width, desc.Height);
		return hr;
	}

	//setTexture�֐�
	//���� 1�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�摜�p�X
	//�߂�l�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@HRESULT
	void SpriteDrawing::setTexture(LPDIRECT3DTEXTURE9 pTex) {
		tex = pTex;
		// �e�N�X�`���T�C�Y�̎擾
		D3DSURFACE_DESC desc;
		if (FAILED(tex->GetLevelDesc(0, &desc))) return;

		setRect(desc.Width, desc.Height);
	}

	//setRect�֐�
	//���� 1�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�e�N�X�`���̉��̒���
	//���� 2�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�e�N�X�`���̏c�̒���
	//�߂�l�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�Ȃ�
	void SpriteDrawing::setRect(LONG right, LONG bottom) {
		rec.top = 0;
		rec.bottom = bottom;
		rec.left = 0;
		rec.right = right;
	}

	//setCenter�֐��@�@�@�@�@�@�@�@�@�@�@�@�@�@
	//���� 1
	//���� 2
	//�߂�l
	void SpriteDrawing::setCenter(float x, float y, float z) {
		cen.x = x;
		cen.y = y;
		cen.z = z;
	}

	//setPosition�֐��@�@�@�@�@�@�@�@�@�@�@�@�@�e�N�X�`���̕`��ʒu���w��
	//���� 1�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�`��ʒux
	//���� 2�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�`��ʒuy
	//���� 3�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�`��ʒuz
	//�߂�l�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�Ȃ�
	void SpriteDrawing::setPosition(float x, float y, float z) {
		pos.x = x;
		pos.y = y;
		pos.z = z;
	}

	//setColor�֐��@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�e�N�X�`����ARGB���w��
	//���� 1�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@D3DCOLOR
	//�߂�l�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�Ȃ�
	void SpriteDrawing::setColor(int a, int r, int g, int b) {
		argb[0] = a;
		argb[1] = r;
		argb[2] = g;
		argb[3] = b;
	}

	//addPositionX�֐�
	//���� 1
	//�߂�l
	float SpriteDrawing::addPositionX(float add) {
		pos.x += add;
		return pos.x;
	}

	//addPositionY�֐�
	//���� 1
	//�߂�l
	float SpriteDrawing::addPositionY(float add) {
		pos.y += add;
		return pos.y;
	}

	//addAlpha�֐�
	//���� 1
	//�߂�l
	bool SpriteDrawing::addAlpha(int alpha) {
		if ((argb[0] + alpha > 255) || (argb[0] + alpha < 0)) {
			return false;
		}
		argb[0] += alpha;
		return true;
	}

	//Draw�֐��@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�e�N�X�`����`��
	//�����@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�Ȃ�
	//�߂�l�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�Ȃ�
	void SpriteDrawing::Draw() {
		g_pSprite->Draw(tex, &rec, &cen, &pos, D3DCOLOR_ARGB(argb[0], argb[1], argb[2], argb[3]));
	}
}