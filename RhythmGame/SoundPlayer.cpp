#include <tchar.h>
#include "SoundPlayer.h"
#include "OggDecoder.h"
#include "OggVorbisMemory.h"
#include "OggVorbisFile.h"
#include "DixSmartPtr.h"
#include "DixComPtr.h"

extern HWND hWnd;

namespace Sound {
	//コンストラクタ
	OggPlayer::OggPlayer(string file) {
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
	OggPlayer::~OggPlayer() {

	}

	//Play関数
	bool OggPlayer::Play(bool loop) {
		if (player.play(loop) == false) {
			return false;
		}
		return true;
	}

	//Pause関数
	void OggPlayer::Pause() {
		player.pause();
	}

	//getState関数
	int OggPlayer::getState(void) {
		return player.getState();
	}

	//コンストラクタ
	WavPlayer::WavPlayer(TCHAR * file) {
		// Waveファイルオープン
		WAVEFORMATEX wFmt;
		char *pWaveData = 0;
		DWORD waveSize = 0;
		if (!openWave(file, wFmt, &pWaveData, waveSize)) return;

		// サウンドデバイス作成
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

		// セカンダリバッファにWaveデータ書き込み
		LPVOID lpvWrite = 0;
		DWORD dwLength = 0;
		if (DS_OK == pDSBuffer->Lock(0, 0, &lpvWrite, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER)) {
			memcpy(lpvWrite, pWaveData, dwLength);
			pDSBuffer->Unlock(lpvWrite, dwLength, NULL, 0);
		}
		delete[] pWaveData; // 元音はもういらない
	}

	//デストラクタ
	WavPlayer::~WavPlayer() {
		pDSBuffer->Release();
	}

	//Play関数
	void WavPlayer::Play(void) {
		pDSBuffer->Play(0, 0, 0);
	}

	//openWave関数
	bool WavPlayer::openWave(TCHAR *filepath, WAVEFORMATEX &waveFormatEx, char** ppData, DWORD &dataSize) {
		if (filepath == 0)
			return false;

		HMMIO hMmio = NULL;
		MMIOINFO mmioInfo;

		// Waveファイルオープン
		memset(&mmioInfo, 0, sizeof(MMIOINFO));
		hMmio = mmioOpen(filepath, &mmioInfo, MMIO_READ);
		if (!hMmio)
			return false; // ファイルオープン失敗

		MMRESULT mmRes;
		MMCKINFO riffChunk;
		riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);
		if (mmRes != MMSYSERR_NOERROR) {
			mmioClose(hMmio, 0);
			return false;
		}

		// フォーマットチャンク検索
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

		// データチャンク検索
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

		// ハンドルクローズ
		mmioClose(hMmio, 0);

		return true;
	}
}