#pragma once
#pragma comment ( lib, "dsound.lib" )
#include <dsound.h>

namespace Wav {
	class WavPlay {
	public:
		WavPlay(TCHAR * file);
		~WavPlay();
		void Play(void);

	private:
		bool openWave(TCHAR *filepath, WAVEFORMATEX &waveFormatEx, char** ppData, DWORD &dataSize);
		IDirectSound8 *pDS8;
		IDirectSoundBuffer8 *pDSBuffer;

	};
}