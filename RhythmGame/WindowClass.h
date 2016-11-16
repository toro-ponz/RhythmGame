#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <stdio.h>
#include "RenderClass.h"

#define SCREEN_WIDTH        1024
#define SCREEN_HEIGHT        768

using namespace Frame;

namespace Win {
	class Window {
	public:
		Window(HINSTANCE, int);
		~Window();
		void Rend();
		bool getRendering();
		Render Render;

	private:
		void loadWindowSettings();
		static LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
		static bool isWindowActive;
		TCHAR *windowName = _T("RhythmGame");
		TCHAR *windowTitle = _T("RhythmGame");
		D3DPRESENT_PARAMETERS D3DPresentParameters;
		bool windowMode;
		bool vSync;
		bool rendering = false;
	};
}