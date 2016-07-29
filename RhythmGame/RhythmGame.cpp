#include "RhythmGame.h"

int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR lpszArgs, int nWinMode) {
	MSG msg;
	Win::Window window(hThisInst, nWinMode);
	while (TRUE) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if (!GetMessage(&msg, NULL, 0, 0)) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			WaitMessage();
		}
	}
	return msg.wParam;
}