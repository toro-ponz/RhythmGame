#pragma once

#include "InputDevice.h"
#include "FrameNumberClass.h"
#include <d3d9.h>
#include <d3dx9core.h>
#include <string>

using namespace Device;
using std::string;

namespace GlobalVariable {
	extern HWND hWnd;
	extern LPD3DXSPRITE Sprite;
	extern LPDIRECT3DDEVICE9 Direct3DDevice9;
	extern LPDIRECT3D9 Direct3D9;
	extern InputDevice *inputDevice;
	extern Frame::FrameNumber frameNumber;

	extern const string dataDirectoryPath;
	extern const string soundEffectDirectoryPath;
	extern const string imageDirectoryPath;
	extern const string colorDirectoryPath;
	extern const string backgorundMusicDirectoryPath;
	extern const string songsDirectoryPath;
	extern const string savedataFilePath;
	extern const string configFilePath;

	extern const string configImageDirectoryPath;
	extern const string creditImageDirectoryPath;
	extern const string openingImageDirectoryPath;
	extern const string playImageDirectoryPath;
	extern const string resultImageDirectoryPath;
	extern const string songselectImageDirectoryPath;

	char* convertConstCharToChar(const char*);
	wchar_t* convertCharToWchar_t(const char*);
	char* convertWchar_tToChar(const wchar_t*);
	string convertTcharToString(const TCHAR*);
	TCHAR* convertStringToTchar(const string);
}

using namespace GlobalVariable;