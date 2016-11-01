#pragma once
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dxguid.lib")

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION  0x0800
#endif

#include <dinput.h>
#include <tchar.h>

namespace Device {
    class Keyboard {
    public:
        Keyboard(HWND);
        ~Keyboard();
		bool getEnabled();
		bool getPushState();
		bool getPushState(int, int = 0);
		bool getPushStateAny(int = 0);
		HRESULT acquire();

    private:
        LPDIRECTINPUT8 DirectInput8 = NULL;
        LPDIRECTINPUTDEVICE8 DirectInputDevice8 = NULL;
        bool keyboardEnabled;
		BYTE keyState[256];
		BYTE oldKeyState[256];
    };
	
	class Joystick {
	public:
		Joystick(HWND);
		~Joystick();
		bool getEnabled();
		bool getPushState();
		bool getPushStateAny(int);
		bool getPushState(int, int);
		void setButtonNum();
		void loadButtonNum();
		HRESULT acquire();

	private:
		bool getPushState(int);
		static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE*, VOID*);
		static BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE*, VOID*);
		static LPDIRECTINPUT8 DirectInput8;
		static LPDIRECTINPUTDEVICE8 DirectInputDevice8;
		DIDEVCAPS diDevCaps;
		DIJOYSTATE js;
		DWORD time;
		bool JoystickEnabled = false;
		bool ButtonState[20] = {};
		bool oldButtonState[20] = {};
		DWORD pressTime[20] = {};
		int ButtonNum[20] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
	};

	class InputDevice {
	public:
		InputDevice(HWND);
		~InputDevice();
		bool getEnabled();
		bool getPushState();
		bool getPushState(int, int = 0);
		bool getPushStateAny(int = 0);
		bool acquire();

	private:
		Keyboard keyboard;
		Joystick joystick;
		int keyboardKey[14] = { DIK_W, DIK_S, DIK_D, DIK_A, DIK_L, DIK_K, DIK_I, DIK_J, DIK_Q, DIK_O, DIK_F1, DIK_F2, DIK_RETURN, DIK_ESCAPE };
	};
}