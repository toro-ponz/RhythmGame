#pragma once
#include "JoystickClass.h"
#include "KeyboardClass.h"

namespace Device {
	class InputDevice {
	public:
		InputDevice(HWND);
		~InputDevice();
		bool getEnabled();
		bool getPushState();
		bool getPushState(int, int = 0);
		bool getPushStateAny(int = 0);
		HRESULT acquire();

	private:
		Joystick * joystick;
		Keyboard * keyboard;
		bool joystickEnabled;
		bool keyboardEnabled;
		
		int keyboardKey[14] = { DIK_W, DIK_S, DIK_D, DIK_A, DIK_L, DIK_K, DIK_I, DIK_J, DIK_Q, DIK_O, DIK_F1, DIK_F2, DIK_RETURN, DIK_ESCAPE };
	};
}