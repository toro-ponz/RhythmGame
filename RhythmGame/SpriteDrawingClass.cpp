#include "SpriteDrawingClass.h"

namespace DxSprite {
	/**
	*  �R���X�g���N�^
	*  @param count<int> �v�f��
	*  @param x<float> �`�b�v��X�����̃s�N�Z���l
	*  @param y<float> �`�b�v��Y�����̃s�N�Z���l(-1��x�Ɠ����l�ƂȂ�)
	*/
	SpriteDrawing::SpriteDrawing(int count, float x, float y) :
			elementCount(count) {
		chipPixelX = x;
		if (y == -1)
			chipPixelY = x;
		else
			chipPixelY = y;
		initialize();
	}

	/**
	*  �R���X�g���N�^
	*  @param path<TCHAR*> �e�N�X�`���̃t�@�C���p�X
	*  @param count<int> �v�f��
	*  @param x<float> �`�b�v��X�����̃s�N�Z���l
	*  @param y<float> �`�b�v��Y�����̃s�N�Z���l(-1��x�Ɠ����l�ƂȂ�)
	*/
	SpriteDrawing::SpriteDrawing(TCHAR* path, int count, float x, float y) :
			elementCount(count){
		chipPixelX = x;
		if (y == -1)
			chipPixelY = x;
		else
			chipPixelY = y;
		initialize();
		setTexture(path);
	}

	/**
	*  �R���X�g���N�^
	*  @param path<string> �e�N�X�`���̃t�@�C���p�X
	*  @param count<int> �v�f��
	*  @param x<float> �`�b�v��X�����̃s�N�Z���l
	*  @param y<float> �`�b�v��Y�����̃s�N�Z���l(-1��x�Ɠ����l�ƂȂ�)
	*/
	SpriteDrawing::SpriteDrawing(string path, int count, float x, float y) :
			elementCount(count){
		chipPixelX = x;
		if (y == -1)
			chipPixelY = x;
		else
			chipPixelY = y;
		initialize();
		setTexture(path);
	}

	/**
	*  �R���X�g���N�^
	*  @param pTex<LPDIRECT3DTEXTURE9> �e�N�X�`���̃t�@�C���p�X
	*  @param count<int> �v�f��
	*  @param x<float> �`�b�v��X�����̃s�N�Z���l
	*  @param y<float> �`�b�v��Y�����̃s�N�Z���l(-1��x�Ɠ����l�ƂȂ�)
	*/
	SpriteDrawing::SpriteDrawing(LPDIRECT3DTEXTURE9 pTex, int count, float x, float y) :
			elementCount(count) {
		chipPixelX = x;
		if (y == -1)
			chipPixelY = x;
		else
			chipPixelY = y;
		initialize();
		setTexture(pTex);
	}
	
	/**
	*  �f�X�g���N�^
	*/
	SpriteDrawing::~SpriteDrawing() {
		if (texture != NULL) {
			texture->Release();
			texture = NULL;
		}
	}

	/**
	*  �e�N�X�`����ǂݍ���œo�^����֐�.
	*  @param path<TCHAR*> �摜�t�@�C���p�X
	*  @return <HRESULT> �摜�t�@�C�����[�h�̐���
	*/
	HRESULT SpriteDrawing::setTexture(TCHAR* path) {
		HRESULT hr = D3DXCreateTextureFromFileEx(Direct3DDevice9,
			path,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			1,
			0,
			D3DFMT_A8R8G8B8,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_FILTER_LINEAR,
			0,
			NULL,
			NULL,
			&texture);

		// �e�N�X�`���T�C�Y�̎擾
		D3DXIMAGE_INFO info;
		D3DXGetImageInfoFromFile(path, &info);
		textureWidth = info.Width;
		textureHeight = info.Height;
		for (int i = 0; i < elementCount; i++) {
			setRectFromPixel(i, 0, textureHeight, 0, textureWidth);
		}
		return hr;
	}

	
	/**
	*  �e�N�X�`����ǂݍ���œo�^����֐�.
	*  @param path<TCHAR*> �摜�t�@�C���p�X
	*  @return <HRESULT> �摜�t�@�C�����[�h�̐���
	*/
	HRESULT SpriteDrawing::setTexture(string path) {
		HRESULT hr = D3DXCreateTextureFromFileEx(Direct3DDevice9,
			convertStringToTchar(path),
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			1,
			0,
			D3DFMT_A8R8G8B8,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_FILTER_LINEAR,
			0,
			NULL,
			NULL,
			&texture);

		// �e�N�X�`���T�C�Y�̎擾
		D3DXIMAGE_INFO info;
		D3DXGetImageInfoFromFile(convertStringToTchar(path), &info);
		textureWidth = info.Width;
		textureHeight = info.Height;
		for (int i = 0; i < elementCount; i++) {
			setRectFromPixel(i, 0, textureHeight, 0, textureWidth);
		}
		return hr;
	}

	/**
	*  �����̃e�N�X�`����o�^����֐�.
	*  @param pTex<LPDIRECT3DTEXTURE9> �e�N�X�`���̃|�C���^
	*/
	void SpriteDrawing::setTexture(LPDIRECT3DTEXTURE9 pTex) {
		texture = pTex;
		// �e�N�X�`���T�C�Y�̎擾
		D3DSURFACE_DESC desc;
		if (FAILED(texture->GetLevelDesc(0, &desc))) return;
		textureWidth = desc.Width;
		textureHeight = desc.Height;
		for (int i = 0; i < elementCount; i++) {
			setRectFromPixel(i, 0, textureHeight, 0, textureWidth);
		}
	}

	/**
	*  �`��͈͂̋�`���w�肷��֐�.
	*  @param suffix<int> �v�f�ԍ�
	*  @param top<LONG> �n�_��Y���W
	*  @param bottom<LONG> �I�_��Y���W
	*  @param left<LONG> �n�_��X���W
	*  @param right<LONG> �I�_��X���W
	*/
	void SpriteDrawing::setRectFromPixel(int suffix, LONG top, LONG bottom, LONG left, LONG right) {
		rect[suffix].top =  top;
		rect[suffix].bottom = bottom;
		rect[suffix].left = left;
		rect[suffix].right =  right;
	}

	/**
	*  �`��͈͂̋�`���^�C���̃C���f�b�N�X�l����w�肷��֐�.
	*  @param suffix<int> �v�f�ԍ�
	*  @param start<int> �͈͊J�n�^�C���̃C���f�b�N�X�l
	*  @param end<int> �͈͏I���^�C���̃C���f�b�N�X�l
	*/
	void SpriteDrawing::setRectFromChip(int suffix, int start, int end) {
		int widthChipCount = (int)(textureWidth / chipPixelX);
		if (end == -1)
			end = start;
		
		float top = chipPixelY * (int)((start - 1) / widthChipCount);
		float bottom = chipPixelY * (int)((end - 1) / widthChipCount + 1);
		float left = chipPixelX * ((start - 1) % widthChipCount);
		float right = chipPixelX * ((end - 1) % widthChipCount + 1);
		
		setRectFromPixel(suffix, (LONG)top, (LONG)bottom, (LONG)left, (LONG)right);
	}

	/**
	*  �`��͈͂̊�_���w�肷��֐�.
	*  @param suffix<int> �v�f�ԍ�
	*  @param x<float> ��_��X�l
	*  @param y<float> ��_��Y�l
	*  @param z<float> ��_��Z�l
	*/
	void SpriteDrawing::setCenter(int suffix, float x, float y, float z) {
		center[suffix].x = x;
		center[suffix].y = y;
		center[suffix].z = z;
	}
	
	/**
	*  �`����W���w�肷��֐�.
	*  @param suffix<int> �v�f�ԍ�
	*  @param x<float> �`����W��X�l
	*  @param y<float> �`����W��Y�l
	*  @param z<float> �`����W��Z�l
	*/
	void SpriteDrawing::setPosition(int suffix, float x, float y, float z) {
		position[suffix].x = x;
		position[suffix].y = y;
		position[suffix].z = z;
	}

	/**
	*  �u�����h����ARGB���w�肷��֐�.
	*  @param suffix<int> �v�f�ԍ�
	*  @param c<D3DCOLOR> D3DCOLOR_ARGB
	*/
	void SpriteDrawing::setColor(int suffix, D3DCOLOR c) {
		color[suffix] = c;
	}

	/**
	*  �`����W���ړ�����֐�.
	*  @param suffix<int> �v�f�ԍ�
	*  @param add<float> X���W�̑���
	*  @return <float> �ړ����X���W
	*/
	float SpriteDrawing::addPositionX(int suffix, float add) {
		position[suffix].x += add;
		return position[suffix].x;
	}

	/**
	*  �`����W���ړ�����֐�.
	*  @param suffix<int> �v�f�ԍ�
	*  @param add<float> Y���W�̑���
	*  @return <float> �ړ����Y���W
	*/
	float SpriteDrawing::addPositionY(int suffix, float add) {
		position[suffix].y += add;
		return position[suffix].y;
	}

	/**
	*  �����x�𑝌�������֐�.
	*  @param suffix<int> �v�f�ԍ�
	*  @param alpha<int> �����x�̑���
	*  @return <bool> �v�Z��̃A���t�@�l������l���ۂ�
	*/
	bool SpriteDrawing::addAlpha(int suffix, int alpha) {
		unsigned int a = ((color[suffix] & 0xff000000) >> 24) + alpha;
		if (a > 255) return false;
		color[suffix] = (a << 24) | (color[suffix] & 0x00ffffff);
		return true;
	}

	/**
	*  �����x�𑝌�������֐�.
	*  @param suffix<int> �v�f�ԍ�
	*  @param interval<DWORD*> 
	*  @param intervalCount<int> 
	*/
	void SpriteDrawing::addAlpha(int suffix, DWORD *interval, int intervalCount) {
		if ((color[suffix] & 0xff000000) >> 24 == 0) {

		}
		else {
			
		}
	}

	/**
	*  �A�j���[�V�����̊Ԋu���w�肷��֐�.
	*  @param suffix<int> �v�f�ԍ�
	*  @param start<int> �A�j���[�V�����̍ŏ��̃`�b�v�ԍ�
	*  @param end<int> �A�j���[�V�����̍Ō�̃`�b�v�ԍ�
	*  @param interval<DWORD*> �e�Ԋu�̔z��̃|�C���^
	*  @param intervalCount<int> interval�z��̗v�f��
	*  @param animationCount<int> �A�j���[�V�����̃R�}��
	*  @param wait<DWORD> �A�j���[�V�����J�n�܂ł̑ҋ@����
	*/
	void SpriteDrawing::setAnimationInterval(int suffix, int start, int end, DWORD *interval, unsigned int intervalCount, unsigned int animationCount, DWORD wait) {
		animationChipStart[suffix] = start;
		animationChipEnd[suffix] = end;
		animationWait[suffix] = wait;
		setRectFromChip(suffix, start);
		if (intervalCount == animationCount) {
			for (int i = 0; i < (int)intervalCount; i++) {
				animationInterval[suffix].push_back(interval[i]);
			}
		}
		else if (intervalCount > animationCount) {
			animationEnabled[suffix] = false;
			return;
		}
		else {
			for (int i = 0; i < (int)intervalCount; i++) {
				animationInterval[suffix].push_back(interval[i]);
			}
			for (int i = (int)intervalCount; i < (int)animationCount; i++) {
				animationInterval[suffix].push_back(interval[0]);
			}
		}
		animationEnabled[suffix] = true;
	}

	/**
	*  �A�j���[�V�������Č����邽�߂�RECT�����v�Z����֐�.
	*  @param suffix<int> �v�f�ԍ�
	*/
	void SpriteDrawing::culcAnimation(int suffix, DWORD time) {
		if (animationOldTime[suffix] == 0)
			animationOldTime[suffix] = time + animationWait[suffix];
		//�O��̎���+����̊Ԋu�̘a�����݂̎��Ԃ����傫���ꍇ���̃R�}��
		if (animationOldTime[suffix] <= time - animationInterval[suffix][animationIntervalSuffix[suffix]]) {
			//animationIntervalSuffix���Ō�Ȃ�ŏ��̃R�}�ɖ߂�
			if (animationIntervalSuffix[suffix] == animationChipEnd[suffix] - animationChipStart[suffix])
				animationIntervalSuffix[suffix] = 0;
			else
				animationIntervalSuffix[suffix]++;
			animationOldTime[suffix] = time;
			//�`�悷��`�b�v�ԍ����v�Z����
			int animationChipNumber = animationIntervalSuffix[suffix] + animationChipStart[suffix];
			setRectFromChip(suffix, animationChipNumber);
		}
	}

	/**
	*  �`���OFF�ɂ���֐�.
	*  @param suffix<int> �v�f�ԍ�
	*/
	void SpriteDrawing::disableDraw(int suffix) {
		drawEnabled[suffix] = false;
	}

	/**
	*  �`���ON�ɂ���֐�.
	*  @param suffix<int> �v�f�ԍ�
	*/
	void SpriteDrawing::enableDraw(int suffix) {
		drawEnabled[suffix] = true;
	}

	/**
	*  �e�N�X�`���̃`�b�v�����w�肷��֐�.
	*  @param x<float> �`�b�v��X�����̃s�N�Z���l
	*  @param y<float> �`�b�v��Y�����̃s�N�Z���l(-1��x�Ɠ����l�ƂȂ�)
	*/
	void SpriteDrawing::setChipPixel(float x, float y) {
		chipPixelX = x;
		if (y == -1)
			chipPixelY = x;
		else
			chipPixelY = y;
	}

	/**
	*  �v�f���𑝂₷�֐�.
	*  @param q<int> ������̗v�f��
	*/
	void SpriteDrawing::reserve(int q) {
		if (elementCount < q) {
			elementCount = q;
			rect.reserve(elementCount);
			center.reserve(elementCount);
			position.reserve(elementCount);
			color.reserve(elementCount);
			animationOldTime.reserve(elementCount);
			animationWait.reserve(elementCount);
			animationInterval = vector<vector<DWORD>>(elementCount);
			animationIntervalSuffix.reserve(elementCount);
			animationChipStart.reserve(elementCount);
			animationChipEnd.reserve(elementCount);
			animationEnabled.reserve(elementCount);
			drawEnabled.reserve(elementCount);
			for (int i = 0; i < elementCount; i++) {
				rect.push_back({ 0, 0, 0, 0 });
				center.push_back({ 0.0f, 0.0f, 0.0f });
				position.push_back({ 0.0f, 0.0f, 0.0f });
				color.push_back(D3DCOLOR_ARGB(255, 255, 255, 255));
				animationOldTime.push_back(0);
				animationWait.push_back(0);
				animationIntervalSuffix.push_back(0);
				animationChipStart.push_back(0);
				animationChipEnd.push_back(0);
				animationEnabled.push_back(false);
				drawEnabled.push_back(true);
				setRectFromPixel(i, 0, textureHeight, 0, textureWidth);
			}
		}
	}

	/**
	*  ����������֐�.
	*/
	void SpriteDrawing::initialize() {
		rect.reserve(elementCount);
		center.reserve(elementCount);
		position.reserve(elementCount);
		color.reserve(elementCount);
		animationOldTime.reserve(elementCount);
		animationWait.reserve(elementCount);
		animationInterval = vector<vector<DWORD>>(elementCount);
		animationIntervalSuffix.reserve(elementCount);
		animationChipStart.reserve(elementCount);
		animationChipEnd.reserve(elementCount);
		animationEnabled.reserve(elementCount);
		drawEnabled.reserve(elementCount);
		for (int i = 0; i < elementCount; i++) {
			rect.push_back({ 0, 0, 0, 0 });
			center.push_back({ 0.0f, 0.0f, 0.0f });
			position.push_back({ 0.0f, 0.0f, 0.0f });
			color.push_back(D3DCOLOR_ARGB(255, 255, 255, 255));
			animationOldTime.push_back(0);
			animationWait.push_back(0);
			animationIntervalSuffix.push_back(0);
			animationChipStart.push_back(0);
			animationChipEnd.push_back(0);
			animationEnabled.push_back(false);
			drawEnabled.push_back(true);
		}
	}

	/**
	*  �e�v�f���X�v���C�g�ɗ������ݕ`�悳����֐�.
	*/
	void SpriteDrawing::Draw() {
		DWORD time = timeGetTime();
		for (int i = 0; i < elementCount; i++) {
			if (drawEnabled[i]) {
				if (animationEnabled[i])
					culcAnimation(i, time);
				Sprite->Draw(texture, &rect[i], &center[i], &position[i], color[i]);
			}
		}
	}
}