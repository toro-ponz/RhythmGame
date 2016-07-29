#include "InputDeviceClass.h"

namespace Device {
	//コンストラクタ
	InputDevice::InputDevice(HWND hWnd) {
		joystick = new Joystick(hWnd);
		keyboard = new Keyboard(hWnd);
		joystickEnabled = joystick->getEnabled();
		keyboardEnabled = keyboard->getEnabled();
	}

	//デストラクタ
	InputDevice::~InputDevice() {
		delete joystick;
		delete keyboard;
	}

	//deviceEnabled関数                         入力デバイスの少なくともどちらかが有効であるかを判定する
	//引数                                      なし
	//戻り値                                    入力デバイスの状態(true or false)
	bool InputDevice::getEnabled() {
		return joystick->getEnabled() || keyboard->getEnabled();
	}

	//getPushState関数                          全キーの入力状態を取得、格納
	//引数                                      なし
	//戻り値                                    成功、失敗(true or false)
	bool InputDevice::getPushState() {
		bool joy = joystick->getPushState();
		bool key = keyboard->getPushState();
		return joy || key;
	}

	//getPushState関数                          指定キーが押されているか(又は押されたか)を判定する
	//引数 1                                    キー番号
	//引数 2                                    モード(0:押されているかどうか, 1:押されたかどうか)
	//戻り値                                    指定キーの入力状態(true or false)
	bool InputDevice::getPushState(int key, int mode) {
		return joystick->getPushState(key, mode) || keyboard->getPushState(keyboardKey[key], mode);
	}

	//getPushStateAny関数                       一つでもキーが押されているか(又は押されたか)を判定する
	//引数                                      モード(0:押されているかどうか, 1:押されたかどうか)
	//戻り値                                    入力デバイスの状態(true or false)
	bool InputDevice::getPushStateAny(int mode) {
		return joystick->getPushStateAny(mode) || keyboard->getPushStateAny(mode);
	}
	
	//acquire関数                               デバイスの再開
	//引数                                      なし
	//戻り値                                    HRESULT
	HRESULT InputDevice::acquire() {
		return SUCCEEDED(joystick->acquire()) || SUCCEEDED(keyboard->acquire());
	}
}