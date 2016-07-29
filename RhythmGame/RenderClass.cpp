#include "RenderClass.h"

extern HWND hWnd;

extern int result[6];

namespace Frame{
	//コンストラクタ
	Render::Render() {
		g_FrameNo = CREDIT1_INIT;
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
		switch (g_FrameNo) {
		case CREDIT1_INIT:
			cr1 = new Credit1Render();
			g_FrameNo = CREDIT1_MAIN;
		case CREDIT1_MAIN:
			cr1->Rend();
			if (g_FrameNo != CREDIT1_MAIN) {
				delete cr1;
				cr1 = NULL;
			}
			break;
		case CREDIT2_INIT:
			cr2 = new Credit2Render();
			g_FrameNo = CREDIT2_MAIN;
		case CREDIT2_MAIN:
			cr2->Rend();
			if (g_FrameNo != CREDIT2_MAIN) {
				delete cr2;
				cr2 = NULL;
			}
			break;
		case TITLE_INIT:
			tr = new TitleRender();
			g_FrameNo = TITLE_MAIN;
		case TITLE_MAIN:
			tr->Rend();
			if (g_FrameNo != TITLE_MAIN) {
				delete tr;
				tr = NULL;
			}
			break;
		case SONG_SELECT_INIT:
			ssr = new SongSelectRender();
			g_FrameNo = SONG_SELECT_MAIN;
			//g_FrameNo = RESULT_INIT;
			//break;
		case SONG_SELECT_MAIN:
			ssr->Rend();
			if (g_FrameNo != SONG_SELECT_MAIN) {
				delete ssr;
				ssr = NULL;
			}
			break;
		case PLAY_EASY_INIT:
			pr = new PlayRender(1);
			g_FrameNo = PLAY_EASY_MAIN;
		case PLAY_EASY_MAIN:
			pr->Rend();
			if (g_FrameNo != PLAY_EASY_MAIN) {
				for (int i = 0; i < 7; i++) {
					result[i] = pr->getResult(i);
				}
				delete pr;
				pr = NULL;
			}
			break;
		case PLAY_NORMAL_INIT:
			pr = new PlayRender(2);
			g_FrameNo = PLAY_NORMAL_MAIN;
		case PLAY_NORMAL_MAIN:
			pr->Rend();
			if (g_FrameNo != PLAY_NORMAL_MAIN) {
				for (int i = 0; i < 7; i++) {
					result[i] = pr->getResult(i);
				}
				delete pr;
				pr = NULL;
			}
			break;
		case PLAY_HARD_INIT:
			pr = new PlayRender(3);
			g_FrameNo = PLAY_HARD_MAIN;
		case PLAY_HARD_MAIN:
			pr->Rend();
			if (g_FrameNo != PLAY_HARD_MAIN) {
				for (int i = 0; i < 7; i++) {
					result[i] = pr->getResult(i);
				}
				delete pr;
				pr = NULL;
			}
			break;
		case RESULT_INIT:
			rr = new ResultRender(result[0], result[1], result[2], result[3], result[4], result[5], result[6]);
			g_FrameNo = RESULT_MAIN;
			break;
		case RESULT_MAIN:
			rr->Rend();
			if (g_FrameNo != RESULT_MAIN) {
				delete rr;
				rr = NULL;
			}
			break;
		case CONFIG_INIT:
			confr = new ConfigRender();
			g_FrameNo = CONFIG_MAIN;
		case CONFIG_MAIN:
			confr->Rend();
			if (g_FrameNo != CONFIG_MAIN) {
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