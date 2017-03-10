#pragma once
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "CreditRenderClass.h"
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
		CreditRender * cr;
		TitleRender * tr;
		SongSelectRender * ssr;
		PlayRender * pr;
		ResultRender * rr;
		ConfigRender * confr;
		int result[7] = { 30, 10, 25, 90, 430, 102, 201390 };
		int level;
		string songName;
		bool autoPlay;
		bool customSkin;
	};
}