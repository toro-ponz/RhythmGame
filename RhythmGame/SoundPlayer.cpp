#include <tchar.h>
#include "SoundPlayer.h"
#include "OggDecoder.h"
#include "OggVorbisMemory.h"
#include "OggVorbisFile.h"
#include "DixSmartPtr.h"
#include "DixComPtr.h"
#include "GlobalVariable.h"

namespace Sound {
	/**
	*  �R���X�g���N�^
	*/
	OggPlayer::OggPlayer(string file) {
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

	/**
	*  �f�X�g���N�^
	*/
	OggPlayer::~OggPlayer() {

	}

	/**
	*  �Đ����J�n����֐�.
	*  @param loop<bool> �Đ������[�v�����邩�ۂ�
	*  @return <bool> ����ɍĐ����J�n���ꂽ���ۂ�
	*/
	bool OggPlayer::Play(bool loop) {
		return player.play(loop);
	}

	/**
	*  �Đ����ꎞ��~����֐�.
	*/
	void OggPlayer::Pause() {
		player.pause();
	}

	/**
	*  �v���C���[�̏�Ԃ�Ԃ��֐�.
	*  @return <STATE> �v���C���[�̏��
	*/
	Dix::PCMPlayer::STATE OggPlayer::getState() {
		return player.getState();
	}

	/**
	*  �R���X�g���N�^
	*/
	WavPlayer::WavPlayer(TCHAR *file) {
		// Wave�t�@�C���I�[�v��
		WAVEFORMATEX wFmt;
		char *pWaveData = 0;
		DWORD waveSize = 0;
		if (!openWave(file, wFmt, &pWaveData, waveSize)) return;

		// �T�E���h�f�o�C�X�쐬
		DirectSoundCreate8(NULL, &pDS8, NULL);
		pDS8->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);

		DSBUFFERDESC DSBufferDesc;
		DSBufferDesc.dwSize = sizeof(DSBUFFERDESC);
		DSBufferDesc.dwFlags = 0;
		DSBufferDesc.dwBufferBytes = waveSize;
		DSBufferDesc.dwReserved = 0;
		DSBufferDesc.lpwfxFormat = &wFmt;
		DSBufferDesc.guid3DAlgorithm = GUID_NULL;

		IDirectSoundBuffer *ptmpBuf = 0;
		pDS8->CreateSoundBuffer(&DSBufferDesc, &ptmpBuf, NULL);
		ptmpBuf->QueryInterface(IID_IDirectSoundBuffer8, (void**)&pDSBuffer);
		ptmpBuf->Release();
		if (pDSBuffer == 0) {
			pDS8->Release();
			return;
		}

		// �Z�J���_���o�b�t�@��Wave�f�[�^��������
		LPVOID lpvWrite = 0;
		DWORD dwLength = 0;
		if (DS_OK == pDSBuffer->Lock(0, 0, &lpvWrite, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER)) {
			memcpy(lpvWrite, pWaveData, dwLength);
			pDSBuffer->Unlock(lpvWrite, dwLength, NULL, 0);
		}
		delete[] pWaveData; // �����͂�������Ȃ�
	}

	/**
	*  �f�X�g���N�^
	*/
	WavPlayer::~WavPlayer() {
		pDSBuffer->Release();
	}

	/**
	*  �Đ����J�n����֐�.
	*/
	void WavPlayer::Play() {
		pDSBuffer->Play(0, 0, 0);
	}

	/**
	*  wave�t�@�C�����J���֐�.
	*  @param filepath<TCHAR*> �Đ�����t�@�C���̃p�X
	*  @param waveFormatEx<WAVEFORMATEX&> 
	*  @param ppData<char**> 
	*  @param dataSize<DWORD&> 
	*  @return <bool> �t�@�C��������ɊJ�������ۂ�
	*/
	bool WavPlayer::openWave(TCHAR *filepath, WAVEFORMATEX &waveFormatEx, char** ppData, DWORD &dataSize) {
		if (filepath == 0)
			return false;

		HMMIO hMmio = NULL;
		MMIOINFO mmioInfo;

		// Wave�t�@�C���I�[�v��
		memset(&mmioInfo, 0, sizeof(MMIOINFO));
		hMmio = mmioOpen(filepath, &mmioInfo, MMIO_READ);
		if (!hMmio)
			return false; // �t�@�C���I�[�v�����s

		MMRESULT mmRes;
		MMCKINFO riffChunk;
		riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);
		if (mmRes != MMSYSERR_NOERROR) {
			mmioClose(hMmio, 0);
			return false;
		}

		// �t�H�[�}�b�g�`�����N����
		MMCKINFO formatChunk;
		formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);
		if (mmRes != MMSYSERR_NOERROR) {
			mmioClose(hMmio, 0);
			return false;
		}
		DWORD fmsize = formatChunk.cksize;
		DWORD size = mmioRead(hMmio, (HPSTR)&waveFormatEx, fmsize);
		if (size != fmsize) {
			mmioClose(hMmio, 0);
			return false;
		}

		mmioAscend(hMmio, &formatChunk, 0);

		// �f�[�^�`�����N����
		MMCKINFO dataChunk;
		dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
		if (mmRes != MMSYSERR_NOERROR) {
			mmioClose(hMmio, 0);
			return false;
		}
		*ppData = new char[dataChunk.cksize];
		size = mmioRead(hMmio, (HPSTR)*ppData, dataChunk.cksize);
		if (size != dataChunk.cksize) {
			delete[] * ppData;
			return false;
		}
		dataSize = size;

		// �n���h���N���[�Y
		mmioClose(hMmio, 0);

		return true;
	}
}