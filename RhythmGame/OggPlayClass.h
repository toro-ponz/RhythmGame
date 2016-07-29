#pragma once
#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dsound.lib" )
#include <dsound.h>
#include <string>
#include "PCMPlayer.h"

using namespace std;

namespace Ogg {
	class OggPlay {
	public:
		OggPlay(string file);
		~OggPlay();
		bool Play(bool);
		void Pause();
		int getState(void);

	private:
		IDirectSound8 * pDS8;
		Dix::PCMPlayer player;
	};
}