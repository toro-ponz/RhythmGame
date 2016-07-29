#include <tchar.h>
#include "OggPlayClass.h"
#include "OggDecoder.h"
#include "OggVorbisMemory.h"
#include "OggVorbisFile.h"
#include "DixSmartPtr.h"
#include "DixComPtr.h"

extern HWND hWnd;

namespace Ogg {
	//�R���X�g���N�^
	OggPlay::OggPlay(string file) {
		// DirectSound�̍쐬
		DirectSoundCreate8(NULL, &pDS8, NULL);
		Dix::Com_ptr< IDirectSound8 > cpDS8(pDS8);
		pDS8->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);

		// Ogg�I�u�W�F�N�g�쐬
		Dix::sp< Dix::OggVorbisMemory > spOggResource(new Dix::OggVorbisMemory(file.c_str()));
		Dix::sp< Dix::OggDecoder > spOggDecoder(new Dix::OggDecoder(spOggResource));

		player.setDevice(cpDS8);
		player.setDecoder(spOggDecoder);
	}

	//�f�X�g���N�^
	OggPlay::~OggPlay() {

	}

	//Play�֐�
	bool OggPlay::Play(bool loop) {
		if (player.play(loop) == false) {
			return false;
		}
		return true;
	}

	//Pause�֐�
	void OggPlay::Pause() {
		player.pause();
	}

	//getState�֐�
	int OggPlay::getState(void) {
		return player.getState();
	}
}