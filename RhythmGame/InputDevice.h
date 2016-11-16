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
		bool getPushState(int, int);
		bool getPushStateAny(int);
		HRESULT acquire();

    private:
        LPDIRECTINPUT8 DirectInput8 = NULL;
        LPDIRECTINPUTDEVICE8 DirectInputDevice8 = NULL;
        bool keyboardEnabled;
		BYTE currentKeyState[256];
		BYTE oldKeyState[256];
    };
	
	class Joystick {
	public:
		Joystick(HWND);
		~Joystick();
		bool getEnabled();
		bool getPushState();
		bool getPushState(int, int);
		bool getPushStateAny(int);
		void setKeyNumber();
		void loadKeyNumber();
		HRESULT acquire();

	private:
		bool getPushState(int);
		static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE*, VOID*);
		static BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE*, VOID*);
		static LPDIRECTINPUT8 DirectInput8;
		static LPDIRECTINPUTDEVICE8 DirectInputDevice8;
		DIDEVCAPS diDevCaps;
		DIJOYSTATE js;
		bool joystickEnabled = false;
		bool currentButtonState[20] = {};
		bool oldButtonState[20] = {};
		DWORD pressTime[20] = {};
		int keyNumber[20] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
	};

	class InputDevice {
	public:
		InputDevice(HWND);
		~InputDevice();
		enum KEY;
		enum KEY_STATE;
		bool getEnabled();
		bool getPushState();
		bool getPushState(KEY, KEY_STATE);
		bool getPushStateAny(KEY_STATE);
		bool acquire();
		
		enum KEY {
			ARROW_UP,
			ARROW_DOWN,
			ARROW_LEFT,
			ARROW_RIGHT,
			BUTTON_UP,
			BUTTON_DOWN,
			BUTTON_LEFT,
			BUTTON_RIGHT,
			BUTTON_SELECT,
			BUTTON_START,
			TRIGGER_LEFT,
			TRIGGER_RIGHT,
			RETURN,
			ESCAPE,
		};
		enum KEY_STATE {
			STATE_HAVE_PUSHED, //âüÇµÇƒÇ¢ÇÈ
			STATE_PUSH //ç°âüÇµÇΩ
		};

	private:
		Keyboard keyboard;
		Joystick joystick;
				
		int keyboardKeys[18] {
			DIK_W,
			DIK_S,
			DIK_A,
			DIK_D,
			DIK_I,
			DIK_K,
			DIK_J,
			DIK_L,
			DIK_F1,
			DIK_F2,
			DIK_Q,
			DIK_O,
			DIK_RETURN,
			DIK_ESCAPE,
			DIK_UP,
			DIK_DOWN,
			DIK_LEFT,
			DIK_RIGHT
		};
		int joystickKeys[14] {
			ARROW_UP,
			ARROW_DOWN,
			ARROW_LEFT,
			ARROW_RIGHT,
			BUTTON_UP,
			BUTTON_DOWN,
			BUTTON_LEFT,
			BUTTON_RIGHT,
			BUTTON_SELECT,
			BUTTON_START,
			TRIGGER_LEFT,
			TRIGGER_RIGHT,
			RETURN,
			ESCAPE,
		};
	};
}