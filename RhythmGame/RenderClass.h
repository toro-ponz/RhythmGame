#pragma once
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define CREDIT1_INIT         10
#define CREDIT1_MAIN         11
#define CREDIT2_MAIN         16
#define TITLE_MAIN           21
#define SONG_SELECT_MAIN     26
#define PLAY_EASY_MAIN       31
#define PLAY_NORMAL_MAIN     36
#define PLAY_HARD_MAIN       41
#define PAUSE_MAIN           46
#define RESULT_MAIN          51
#define CONFIG_MAIN          56

#include <Windows.h>
#include "Credit1RenderClass.h"
#include "Credit2RenderClass.h"
#include "TitleRenderClass.h"
#include "SongSelectRenderClass.h"
#include "PlayRenderClass.h"
#include "ResultRenderClass.h"
#include "ConfigRenderClass.h"

extern BYTE FrameNumber;

namespace Frame {
	class Render {
	public:
		Render();
		~Render();
		void Rend(void);

	private:
		Credit1Render * cr1;
		Credit2Render * cr2;
		TitleRender * tr;
		SongSelectRender * ssr;
		PlayRender * pr;
		ResultRender * rr;
		ConfigRender * confr;

		int result[7] = { 30, 10, 25, 90, 430, 102, 201390 };

	};
}