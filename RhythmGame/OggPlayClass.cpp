#include <tchar.h>
#include "OggPlayClass.h"
#include "OggDecoder.h"
#include "OggVorbisMemory.h"
#include "OggVorbisFile.h"
#include "DixSmartPtr.h"
#include "DixComPtr.h"

extern HWND hWnd;

namespace Ogg {
	//コンストラクタ
	OggPlay::OggPlay(string file) {
		// DirectSoundの作成
		DirectSoundCreate8(NULL, &pDS8, NULL);
		Dix::Com_ptr< IDirectSound8 > cpDS8(pDS8);
		pDS8->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);

		// Oggオブジェクト作成
		Dix::sp< Dix::OggVorbisMemory > spOggResource(new Dix::OggVorbisMemory(file.c_str()));
		Dix::sp< Dix::OggDecoder > spOggDecoder(new Dix::OggDecoder(spOggResource));

		player.setDevice(cpDS8);
		player.setDecoder(spOggDecoder);
	}

	//デストラクタ
	OggPlay::~OggPlay() {

	}

	//Play関数
	bool OggPlay::Play(bool loop) {
		if (player.play(loop) == false) {
			return false;
		}
		return true;
	}

	//Pause関数
	void OggPlay::Pause() {
		player.pause();
	}

	//getState関数
	int OggPlay::getState(void) {
		return player.getState();
	}
}