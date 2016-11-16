#pragma once
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "Credit1RenderClass.h"
#include "Credit2RenderClass.h"
#include "TitleRenderClass.h"
#include "SongSelectRenderClass.h"
#include "PlayRenderClass.h"
#include "ResultRenderClass.h"
#include "ConfigRenderClass.h"
#include <Windows.h>


namespace Frame {
	class Render {
	public:
		Render();
		~Render();
		void Rend();

	private:
		Credit1Render * cr1;
		Credit2Render * cr2;
		TitleRender * tr;
		SongSelectRender * ssr;
		PlayRender * pr;
		ResultRender * rr;
		ConfigRender * confr;
		int result[7] = { 30, 10, 25, 90, 430, 102, 201390 };
		int level;
		string songName;
	};
}