#include "SpriteChipClass.h"

namespace Sprite {
	//�R���X�g���N�^
	//���� 1                                    �v�f��
	//�ߒl                                      �Ȃ�
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

	//�R���X�g���N�^
	//���� 1                                    �摜�p�X
	//���� 2                                    �v�f��
	//�ߒl                                      �Ȃ�
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

	//�f�X�g���N�^
	//����                                      �Ȃ�
	//�ߒl                                      �Ȃ�
	SpriteChip::~SpriteChip() {
		if (tex != NULL) {
			tex->Release();
			tex = NULL;
		}
	}

	//setTexture�֐�                            �e�N�X�`�����Z�b�g����
	//���� 1                                    �摜�p�X
	//�ߒl                                      HRESULT
	HRESULT SpriteChip::setTexture(LPCWSTR path) {
		HRESULT hr = D3DXCreateTextureFromFile(gl_lpD3ddev, path, &tex);
		// �e�N�X�`���T�C�Y�̎擾
		D3DSURFACE_DESC desc;
		if (FAILED(tex->GetLevelDesc(0, &desc))) return hr;

		texWidth = desc.Width;
		texHeight = desc.Height;
		for (int i = 0; i < num; i++) {
			setRect(i, texWidth, texHeight);
		}
		return hr;
	}

	//setTexture�֐�                            �e�N�X�`�����Z�b�g����
	//���� 1                                    �e�N�X�`���̃|�C���^
	//�ߒl                                      �Ȃ�
	void SpriteChip::setTexture(LPDIRECT3DTEXTURE9 pTex) {
		tex = pTex;
		// �e�N�X�`���T�C�Y�̎擾
		D3DSURFACE_DESC desc;
		if (FAILED(tex->GetLevelDesc(0, &desc))) return;

		texWidth = desc.Width;
		texHeight = desc.Height;
		for (int i = 0; i < num; i++) {
			setRect(i, texWidth, texHeight);
		}
	}

	//setRect�֐�                               RECT�\���̂��Z�b�g����
	//���� 1                                    �Y����
	//���� 2                                    RECT.right
	//���� 3                                    RECT.bottom
	//���� 4                                    RECT.left
	//���� 5                                    RECT.top
	//�ߒl                                      �Ȃ�
	void SpriteChip::setRect(int sub, LONG right, LONG bottom, LONG left, LONG top) {
		rec[sub].top =  top;
		rec[sub].bottom = bottom;
		rec[sub].left = left;
		rec[sub].right =  right;
	}

	//setRectC�֐�                              RECT�\���̂��Z�b�g����
	//���� 1                                    �Y����
	//���� 2                                    �摜�`�b�v�̊J�n�C���f�b�N�X�l
	//���� 3                                    �摜�`�b�v�̏I���C���f�b�N�X�l
	//�ߒl                                      �Ȃ�
	void SpriteChip::setRectC(int sub, int start, int end) {
		int wTile = (int)(texWidth / tilePixel);
		float left = tilePixel * (start - (wTile * (int)((start - 1) / wTile)) - 1);
		float top = tilePixel * (int)((start - 1) / wTile);
		float right = tilePixel * (end - (wTile * (int)((end - 1) / wTile)));
		float bottom = tilePixel * (int)((end - 1) / wTile + 1);
		setRect(sub, (UINT)right, (UINT)bottom, (UINT)left, (UINT)top);
	}

	//setCenter�֐�                             �e�N�X�`�`����Center���Z�b�g����
	//���� 1                                    �Y����
	//���� 2                                    Center.x
	//���� 3                                    Center.y
	//���� 4                                    Center.z
	//�ߒl                                      �Ȃ�
	void SpriteChip::setCenter(int sub, float x, float y, float z) {
		cen[sub].x = x;
		cen[sub].y = y;
		cen[sub].z = z;
	}

	//setPosition�֐�                           �e�N�X�`���̕`��ʒu���w��
	//���� 1                                    Position.x
	//���� 2                                    Position.y
	//���� 3                                    Position.z
	//�ߒl                                      �Ȃ�
	void SpriteChip::setPosition(int sub, float x, float y, float z) {
		pos[sub].x = x;
		pos[sub].y = y;
		pos[sub].z = z;
	}

	//setColor�֐�                              �e�N�X�`����ARGB���w��
	//���� 1                                    D3DCOLOR_ARGB
	//���� 2                                    �Y����
	//�ߒl                                      �Ȃ�
	void SpriteChip::setColor(D3DCOLOR c, int sub) {
		color[sub] = c;
	}

	//addPositionX�֐�                          �`��ʒu�����炷
	//���� 1                                    X���W�̑���
	//���� 2                                    �Y����
	//�ߒl                                      �v�Z��̕`��ʒu��X���W
	float SpriteChip::addPositionX(float add, int sub) {
		pos[sub].x += add;
		return pos[sub].x;
	}

	//addPositionY�֐�                          �`��ʒu�����炷
	//���� 1                                    Y���W�̑���
	//���� 2                                    �Y����
	//�ߒl                                      �v�Z��̕`��ʒu��X���W
	float SpriteChip::addPositionY(float add, int sub) {
		pos[sub].y += add;
		return pos[sub].y;
	}

	//addAlpha�֐�                              �����x��ς���
	//���� 1                                    �s�����x�̑���
	//���� 2                                    �Y����
	//�ߒl                                      �s�����x�͈̔�(0~255)�𒴂����ꍇfalse��Ԃ�
	bool SpriteChip::addAlpha(int alpha, int sub) {
		unsigned int a = ((color[sub] & 0xff000000) >> 24) + alpha;
		if (a > 256) return false;
		color[sub] = (a << 24) | (color[sub] & 0x00ffffff);
		return true;
	}

	//disableDraw�֐�                           �`���OFF�ɂ���
	//���� 1                                    �Y����
	//�ߒl                                      �Ȃ�
	void SpriteChip::disableDraw(int sub) {
		draw[sub] = false;
	}

	//enableDraw�֐�                            �`���ON�ɂ���
	//���� 1                                    �Y����
	//�ߒl                                      �Ȃ�
	void SpriteChip::enableDraw(int sub) {
		draw[sub] = true;
	}

	//setTile�֐�                               �摜�`�b�v��1�̕����Z�b�g����
	//���� 1                                    �`�b�v��
	//�ߒl                                      �Ȃ�
	void SpriteChip::setTile(float t) {
		tilePixel = t;
	}

	//Draw�֐�                                  �e�N�X�`����`�悷��
	//����                                      �Ȃ�
	//�ߒl                                      �Ȃ�
	void SpriteChip::Draw() {
		for (int i = 0; i < num; i++) {
			if (draw[i]) {
				g_pSprite->Draw(tex, &rec[i], &cen[i], &pos[i], color[i]);
			}
		}
	}
}