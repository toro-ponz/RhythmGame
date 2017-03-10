#include "Global.h"
#include "resource.h"
#include <vector>

namespace Global {
	HWND hWnd;
	LPD3DXSPRITE Sprite;
	LPDIRECT3DDEVICE9 Direct3DDevice9;
	LPDIRECT3D9 Direct3D9 = NULL;
	InputDevice *inputDevice;
	Frame::FrameNumber frameNumber;

	const string dataDirectoryPath = "./Data/";
	const string soundEffectDirectoryPath = dataDirectoryPath + "sound-effect/";
	const string imageDirectoryPath = dataDirectoryPath + "img/";
	const string colorDirectoryPath = dataDirectoryPath + "img/color/";
	const string backgorundMusicDirectoryPath = dataDirectoryPath + "bgm/";
	const string songsDirectoryPath = dataDirectoryPath + "songs/";
	const string savedataFilePath = dataDirectoryPath + "savedata.dat";
	const string configFilePath = dataDirectoryPath + "conf.ini";

	const string configImageDirectoryPath = imageDirectoryPath + "config/";
	const string creditImageDirectoryPath = imageDirectoryPath + "credit/";
	const string openingImageDirectoryPath = imageDirectoryPath + "opening/";
	const string playImageDirectoryPath = imageDirectoryPath + "play/";
	const string resultImageDirectoryPath = imageDirectoryPath + "result/";
	const string songselectImageDirectoryPath = imageDirectoryPath + "songselect/";

	const string resourcePath = "";

	const string fontName = "���C���I";
	
	/**
	*  const char*�^�̕������char*�^�ɕϊ�����֐�.
	*  @param letters<const char*> �ϊ����镶����̃|�C���^
	*  @return <char*> �ϊ����ꂽ������̃|�C���^
	*/
	char* convertConstCharToChar(const char* letters) {
		size_t length = strlen(letters);
		char *result = new char[length + 1];
		if (!strcpy_s(result, length + 1, letters))
			return result;
		else
			return NULL;
	}

	/**
	*  const char*�^�̕������wchar_t*�^�ɕϊ�����֐�.
	*  @param letters<const char*> �ϊ����镶����̃|�C���^
	*  @return <wchar_t*> �ϊ����ꂽ������̃|�C���^
	*/
	wchar_t* convertCharToWchar_t(const char *letters) {
		size_t length = strlen(letters);
		wchar_t* result = new wchar_t[length + 1];
		setlocale(LC_ALL, "japanese");
		if (!mbstowcs_s(&length, result, length + 1, letters, _TRUNCATE))
			return result;
		else
			return NULL;
	}

	/**
	*  const wchar_t*�^�̕������char*�^�ɕϊ�����֐�.
	*  @param letters<const wchar_t*> �ϊ����镶����̃|�C���^
	*  @return <char*> �ϊ����ꂽ������̃|�C���^
	*/
	char* convertWchar_tToChar(const wchar_t *letters) {
		size_t length = (wcslen(letters) + 1) * 2;
		char *result = new char[length + 1];
		setlocale(LC_ALL, "japanese");
		if (!wcstombs_s(&length, result, length, letters, _TRUNCATE))
			return result;
		else
			return NULL;
	}

	/**
	*  const TCHAR*�^�̕������string�^�ɕϊ�����֐�.
	*  @param letters<const TCHAR*> �ϊ����镶����̃|�C���^
	*  @return <string> �ϊ����ꂽ������
	*/
	string convertTcharToString(const TCHAR *letters) {
#ifdef UNICODE
		return convertWchar_tToChar(letters);
#else
		return letters;
#endif
	}

	/**
	*  const string�^�̕������TCHAR*�^�ɕϊ�����֐�.
	*  @param letters<const string> �ϊ����镶����
	*  @return <TCHAR*> �ϊ����ꂽ������̃|�C���^
	*/
	TCHAR* convertStringToTchar(const string letters) {
#ifdef UNICODE
		return convertCharToWchar_t(letters.c_str());
#else
		return convertConstCharToChar(letters.c_str());
#endif
	}
	
	/**
	*  SE���Đ�����֐�.
	*  @param path<string> �t�@�C���p�X
	*/
	void playSoundEffectFromFile(string path) {
		if (path != "") {
			PlaySound(convertStringToTchar(path), NULL, SND_FILENAME | SND_ASYNC);
		}
	}

	/**
	*  SE���Đ�����֐�.
	*  @param suffix<int> ���\�[�X�ԍ�
	*/
	void playSoundEffectFromResource(int suffix) {
		switch (suffix) {
		case 1:
			PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_RESOURCE | SND_ASYNC);
			break;
		case 2:
			PlaySound(MAKEINTRESOURCE(IDR_WAVE2), NULL, SND_RESOURCE | SND_ASYNC);
			break;
		case 3:
			PlaySound(MAKEINTRESOURCE(IDR_WAVE3), NULL, SND_RESOURCE | SND_ASYNC);
			break;
		case 4:
			PlaySound(MAKEINTRESOURCE(IDR_WAVE4), NULL, SND_RESOURCE | SND_ASYNC);
			break;
		case 5:
			PlaySound(MAKEINTRESOURCE(IDR_WAVE5), NULL, SND_RESOURCE | SND_ASYNC);
			break;
		case 6:
			PlaySound(MAKEINTRESOURCE(IDR_WAVE6), NULL, SND_RESOURCE | SND_ASYNC);
			break;
		case 7:
			PlaySound(MAKEINTRESOURCE(IDR_WAVE7), NULL, SND_RESOURCE | SND_ASYNC);
			break;
		case 8:
			PlaySound(MAKEINTRESOURCE(IDR_WAVE8), NULL, SND_RESOURCE | SND_ASYNC);
			break;
		}
	}
}