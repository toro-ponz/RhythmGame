#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <stdio.h>
#include "RenderClass.h"

#define WINMODE             TRUE
#define SCREEN_WIDTH        1024
#define SCREEN_HEIGHT        768

using namespace Frame;

namespace Win {
	class Window {
	public:
		Window(HINSTANCE, int);
		~Window();
		static Render * Render;

	private:
		static LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
		static bool g_appActive;
		TCHAR szWinName[11] = _T("RhythmGame");
		TCHAR szWinTitle[11] = _T("RhythmGame");
		D3DPRESENT_PARAMETERS gl_d3dpp;
	};
}