#include "RenderClass.h"
#include "GlobalVariable.h"

namespace Frame{
	/**
	*  コンストラクタ
	*/
	Render::Render() {

	}

	/**
	*  デストラクタ
	*/
	Render::~Render() {
		if (cr1 != NULL) {
			delete cr1;
			cr1 = NULL;
		}
		if (cr2 != NULL) {
			delete cr2;
			cr2 = NULL;
		}
		if (tr != NULL) {
			delete tr;
			tr = NULL;
		}
		if (ssr != NULL) {
			delete ssr;
			ssr = NULL;
		}
		if (pr != NULL) {
			delete pr;
			pr = NULL;
		}
		if (rr != NULL) {
			delete rr;
			rr = NULL;
		}
		if (confr != NULL) {
			delete confr;
			confr = NULL;
		}
	}

	/**
	*  描画する関数.
	*/
	void Render::Rend(void) {
		switch (frameNumber.getFrameNumber()) {
		case FrameNumber::FRAME_NUMBER::CREDIT1_INIT:
			cr1 = new Credit1Render();
			frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::CREDIT1_MAIN);
		case FrameNumber::FRAME_NUMBER::CREDIT1_MAIN:
			cr1->Rend();
			if (!frameNumber.compareFrameNumber(FrameNumber::FRAME_NUMBER::CREDIT1_MAIN)) {
				delete cr1;
				cr1 = NULL;
			}
			break;
		case FrameNumber::FRAME_NUMBER::CREDIT2_INIT:
			cr2 = new Credit2Render();
			frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::CREDIT2_MAIN);
		case FrameNumber::FRAME_NUMBER::CREDIT2_MAIN:
			cr2->Rend();
			if (!frameNumber.compareFrameNumber(FrameNumber::FRAME_NUMBER::CREDIT2_MAIN)) {
				delete cr2;
				cr2 = NULL;
			}
			break;
		case FrameNumber::FRAME_NUMBER::TITLE_INIT:
			tr = new TitleRender();
			frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::TITLE_MAIN);
		case FrameNumber::FRAME_NUMBER::TITLE_MAIN:
			tr->Rend();
			if (!frameNumber.compareFrameNumber(FrameNumber::FRAME_NUMBER::TITLE_MAIN)) {
				delete tr;
				tr = NULL;
			}
			break;
		case FrameNumber::FRAME_NUMBER::SONG_SELECT_INIT:
			ssr = new SongSelectRender();
			frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::SONG_SELECT_MAIN);
		case FrameNumber::FRAME_NUMBER::SONG_SELECT_MAIN:
			ssr->Rend();
			if (!frameNumber.compareFrameNumber(FrameNumber::FRAME_NUMBER::SONG_SELECT_MAIN)) {
				level = ssr->getSelectedSheetLevel();
				songName = ssr->getSelectedSongName();
				delete ssr;
				ssr = NULL;
			}
			break;
		case FrameNumber::FRAME_NUMBER::PLAY_INIT:
			pr = new PlayRender(songName, level);
			frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::PLAY_MAIN);
		case FrameNumber::FRAME_NUMBER::PLAY_MAIN:
			pr->Rend();
			if (!frameNumber.compareFrameNumber(FrameNumber::FRAME_NUMBER::PLAY_MAIN)) {
				for (int i = 0; i < 7; i++) {
					result[i] = pr->getResult(i);
				}
				delete pr;
				pr = NULL;
			}
			break;
		case FrameNumber::FRAME_NUMBER::RESULT_INIT:
			rr = new ResultRender(result[0], result[1], result[2], result[3], result[4], result[5], result[6]);
			frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::RESULT_MAIN);
			break;
		case FrameNumber::FRAME_NUMBER::RESULT_MAIN:
			rr->Rend();
			if (!frameNumber.compareFrameNumber(FrameNumber::FRAME_NUMBER::RESULT_MAIN)) {
				delete rr;
				rr = NULL;
			}
			break;
		case FrameNumber::FRAME_NUMBER::CONFIG_INIT:
			confr = new ConfigRender();
			frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::CONFIG_MAIN);
		case FrameNumber::FRAME_NUMBER::CONFIG_MAIN:
			confr->Rend();
			if (!frameNumber.compareFrameNumber(FrameNumber::FRAME_NUMBER::CONFIG_MAIN)) {
				delete confr;
				confr = NULL;
			}
			break;
		default:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		}
	}
}