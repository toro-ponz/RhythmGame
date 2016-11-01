#include "RenderClass.h"

extern HWND hWnd;

extern int result[6];

namespace Frame{
	//コンストラクタ
	Render::Render() {
		FrameNumber = CREDIT1_INIT;
	}

	//デストラクタ
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

	void Render::Rend(void) {
		switch (FrameNumber) {
		case CREDIT1_INIT:
			cr1 = new Credit1Render();
			FrameNumber = CREDIT1_MAIN;
		case CREDIT1_MAIN:
			cr1->Rend();
			if (FrameNumber != CREDIT1_MAIN) {
				delete cr1;
				cr1 = NULL;
			}
			break;
		case CREDIT2_INIT:
			cr2 = new Credit2Render();
			FrameNumber = CREDIT2_MAIN;
		case CREDIT2_MAIN:
			cr2->Rend();
			if (FrameNumber != CREDIT2_MAIN) {
				delete cr2;
				cr2 = NULL;
			}
			break;
		case TITLE_INIT:
			tr = new TitleRender();
			FrameNumber = TITLE_MAIN;
		case TITLE_MAIN:
			tr->Rend();
			if (FrameNumber != TITLE_MAIN) {
				delete tr;
				tr = NULL;
			}
			break;
		case SONG_SELECT_INIT:
			ssr = new SongSelectRender();
			FrameNumber = SONG_SELECT_MAIN;
			//FrameNumber = RESULT_INIT;
			//break;
		case SONG_SELECT_MAIN:
			ssr->Rend();
			if (FrameNumber != SONG_SELECT_MAIN) {
				delete ssr;
				ssr = NULL;
			}
			break;
		case PLAY_EASY_INIT:
			pr = new PlayRender(1);
			FrameNumber = PLAY_EASY_MAIN;
		case PLAY_EASY_MAIN:
			pr->Rend();
			if (FrameNumber != PLAY_EASY_MAIN) {
				for (int i = 0; i < 7; i++) {
					result[i] = pr->getResult(i);
				}
				delete pr;
				pr = NULL;
			}
			break;
		case PLAY_NORMAL_INIT:
			pr = new PlayRender(2);
			FrameNumber = PLAY_NORMAL_MAIN;
		case PLAY_NORMAL_MAIN:
			pr->Rend();
			if (FrameNumber != PLAY_NORMAL_MAIN) {
				for (int i = 0; i < 7; i++) {
					result[i] = pr->getResult(i);
				}
				delete pr;
				pr = NULL;
			}
			break;
		case PLAY_HARD_INIT:
			pr = new PlayRender(3);
			FrameNumber = PLAY_HARD_MAIN;
		case PLAY_HARD_MAIN:
			pr->Rend();
			if (FrameNumber != PLAY_HARD_MAIN) {
				for (int i = 0; i < 7; i++) {
					result[i] = pr->getResult(i);
				}
				delete pr;
				pr = NULL;
			}
			break;
		case RESULT_INIT:
			rr = new ResultRender(result[0], result[1], result[2], result[3], result[4], result[5], result[6]);
			FrameNumber = RESULT_MAIN;
			break;
		case RESULT_MAIN:
			rr->Rend();
			if (FrameNumber != RESULT_MAIN) {
				delete rr;
				rr = NULL;
			}
			break;
		case CONFIG_INIT:
			confr = new ConfigRender();
			FrameNumber = CONFIG_MAIN;
		case CONFIG_MAIN:
			confr->Rend();
			if (FrameNumber != CONFIG_MAIN) {
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