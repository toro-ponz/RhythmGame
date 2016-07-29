#pragma once
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dxguid.lib")

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION  0x0900
#endif

#include <dinput.h>
#include <tchar.h>

namespace Device {
	class Joystick {
	public:
		Joystick(HWND);
		~Joystick();
		bool getEnabled();
		bool getPushState();
		bool getPushStateAny(int);
		bool getPushState(int, int);
		void setButtonNum();
		HRESULT acquire();

	private:
		static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE*, VOID*);
		static BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE*, VOID*);
		bool getPushState(int);
		static LPDIRECTINPUT8 lpDI;
		static LPDIRECTINPUTDEVICE8 lpDIDevice;
		DIDEVCAPS diDevCaps;
		DIJOYSTATE js;
		DWORD time;
		bool JoystickEnabled = false;
		bool ButtonState[20] = {};
		bool oldButtonState[20] = {};
		DWORD pressTime[20] = {};
		int ButtonNum[20] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

	};
}