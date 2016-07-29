#pragma once
#pragma comment(lib, "dinput8.lib")

#include <dinput.h>

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
        LPDIRECTINPUT8 g_pDInput = NULL;
        LPDIRECTINPUTDEVICE8 g_pDIDevice = NULL;

        bool keyboardEnabled;
		BYTE keyState[256];
		BYTE oldKeyState[256];
    };
}