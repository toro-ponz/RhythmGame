#pragma once
#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dsound.lib" )
#include <dsound.h>
#include <string>
#include "PCMPlayer.h"

using namespace std;

namespace Sound {
	class OggPlayer {
	public:
		OggPlayer(string file);
		~OggPlayer();
		bool Play(bool);
		void Pause();
		int getState();

	private:
		IDirectSound8 * pDS8;
		Dix::PCMPlayer player;
	};

	class WavPlayer {
	public:
		WavPlayer(TCHAR*);
		~WavPlayer();
		void Play();

	private:
		bool openWave(TCHAR*, WAVEFORMATEX&, char**, DWORD&);
		IDirectSound8 *pDS8;
		IDirectSoundBuffer8 *pDSBuffer;
	};
}