#include "InputDevice.h"

namespace Device {
	/**
	*  コンストラクタ
	*  @param hWnd<HWND> ウィンドウハンドル
	*/
    Keyboard::Keyboard(HWND hWnd) {
        if (SUCCEEDED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DirectInput8, NULL))) {
            if (SUCCEEDED(DirectInput8->CreateDevice(GUID_SysKeyboard, &DirectInputDevice8, NULL))) {
				DirectInputDevice8->SetDataFormat(&c_dfDIKeyboard);
				DirectInputDevice8->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
				DirectInputDevice8->Acquire();
				keyboardEnabled = true;
            }
            else {
                keyboardEnabled = false;
            }
        }
        else {
            keyboardEnabled = false;
        }
    }

	/**
	*  デストラクタ
	*/
    Keyboard::~Keyboard() {
		if (keyboardEnabled) {
			DirectInputDevice8->Unacquire();
		}
		if (DirectInputDevice8 != NULL) {
			DirectInputDevice8->Release();
			DirectInputDevice8 = NULL;
		}
		if (DirectInput8 != NULL) {
			DirectInput8->Release();
			DirectInput8 = NULL;
		}
    }
	
	/**
	*  キーボードが接続されており、正しく動作するか判定する関数.
	*  @return <bool> keyboardEnabled
	*/
	bool Keyboard::getEnabled() {
		return keyboardEnabled;
	}

	/**
	*  全ボタンの押下状態を保存する関数.
	*  @return <bool> 正常に保存できればtrue, そうでなければfalse
	*/
	bool Keyboard::getPushState() {
		memcpy(oldKeyState, keyState, sizeof(keyState));
		if (keyboardEnabled) {
			if (SUCCEEDED(DirectInputDevice8->GetDeviceState(sizeof(keyState), (LPVOID)&keyState))) {
				return true;
			}
			else {
				DirectInputDevice8->Acquire();
				if (SUCCEEDED(DirectInputDevice8->GetDeviceState(sizeof(keyState), (LPVOID)&keyState))) {
					return true;
				}
				else {
					return false;
				}
			}
		}
		else {
			return false;
		}
	}
	
	/**
	*  指定ボタンが押されているかどうか判定する関数.
	*  @param key<int> 0:↑, 1:↓, 2:→, 3:←, 4:決定キー, 5:キャンセルキー......
	*  @param mode<int> 0:押されているかどうか, 1:今押されたかどうか
	*  @return <bool> 判定結果
	*/
	bool Keyboard::getPushState(int key, int mode) {
		if (keyboardEnabled) {
			if (!mode) {
				if (keyState[key] & 0x80) {
					return true;
				}
				else {
					return false;
				}
			}
			else {
				if ((keyState[key] & 0x80) && !(oldKeyState[key] & 0x80)) {
					return true;
				}
				else {
					return false;
				}
			}
		}
		else {
			return false;
		}
	}

	/**
	*  1つでもボタンが押されているかどうか判定する関数.
	*  @param mode<int> 0:押されているかどうか, 1:今押されたかどうか
	*  @return <bool> 判定結果
	*/
	bool Keyboard::getPushStateAny(int mode) {
		if (keyboardEnabled) {
			if (!mode) {
				for (int i = 0; i < sizeof(keyState); i++) {
					if (keyState[i] & 0x80) {
						return true;
					}
				}
				return false;
			}
			else {
				for (int i = 0; i < sizeof(keyState); i++) {
					if ((keyState[i] & 0x80) && !(oldKeyState[i] & 0x80)) {
						return true;
					}
				}
				return false;
			}
			return true;
		}
		else {
			return false;
		}
	}

	/**
	*  入力デバイスを再開する関数.
	*  @return <HRESULT> デバイスが有効ならAcquire関数の戻り値, 無効ならE_FAIL
	*/
	HRESULT Keyboard::acquire() {
		if (keyboardEnabled) {
			return DirectInputDevice8->Acquire();
		}
		else {
			return E_FAIL;
		}
	}
	
	/**
	*  コンストラクタ
	*  @param hWnd<HWND> ウィンドウハンドル
	*/
	Joystick::Joystick(HWND hWnd) {
		HINSTANCE hInst = GetModuleHandle(NULL);
		//DirectInput8インターフェイスの取得
		if (SUCCEEDED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DirectInput8, NULL))) {
			if (SUCCEEDED(DirectInput8->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY))) {
				if (DirectInputDevice8 != NULL) {
					DirectInputDevice8->SetDataFormat(&c_dfDIJoystick);
					// 協調モードの設定
					DirectInputDevice8->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
					diDevCaps.dwSize = sizeof(DIDEVCAPS);
					DirectInputDevice8->GetCapabilities(&diDevCaps);
					DirectInputDevice8->EnumObjects(EnumAxesCallback, (VOID*)hWnd, DIDFT_AXIS);
					// 入力の開始
					DirectInputDevice8->Poll();
					DirectInputDevice8->Acquire();
					loadButtonNum();
					JoystickEnabled = true;
				}
				else {
					JoystickEnabled = false;
				}
			}
			else {
				JoystickEnabled = false;
			}
		}
		else {
			JoystickEnabled = false;
		}
	}

	/**
	*  デストラクタ
	*/
	Joystick::~Joystick() {
		if (JoystickEnabled) {
			DirectInputDevice8->Unacquire();
		}
		if (DirectInputDevice8 != NULL) {
			DirectInputDevice8->Release();
			DirectInputDevice8 = NULL;
		}
		if (DirectInput8 != NULL) {
			DirectInput8->Release();
			DirectInput8 = NULL;
		}
	}

	/**
	*  ジョイスティックが接続されており、正しく動作するか判定する関数.
	*  @return <bool> JoystickEnabled
	*/
	bool Joystick::getEnabled() {
		return JoystickEnabled;
	}

	/**
	*  全ボタンの押下状態を保存する関数.
	*  @return <bool> JoystickEnabled
	*/
	bool Joystick::getPushState() {
		if (JoystickEnabled) {
			time = timeGetTime();
			for (int i = 0; i < 20; i++) {
				//前回の状態をold変数に保存する
				oldButtonState[i] = ButtonState[i];
				//今回の状態を変数に保存する
				ButtonState[i] = getPushState(i);
			}
			return true;
		}
		else {
			return false;
		}
	}
	
	/**
	*  指定ボタンが押されているかどうか判定する関数.
	*  @param key<int> 0:↑, 1:↓, 2:→, 3:←, 4:決定キー, 5:キャンセルキー......
	*  @return <bool> 判定結果
	*/
	bool Joystick::getPushState(int key) {
		if (JoystickEnabled) {
			bool result = false;
			HRESULT hr = DirectInputDevice8->GetDeviceState(sizeof(DIJOYSTATE), &js);
			if (key == 0) {
				if (js.lY == -1000) result = true;
			}
			else if (key == 1) {
				if (js.lY == 1000) result = true;
			}
			else if (key == 2) {
				if (js.lX == 1000) result = true;
			}
			else if (key == 3) {
				if (js.lX == -1000) result = true;
			}
			else {
				if (js.rgbButtons[ButtonNum[key] - 4] & 0x80) result = true;
			}
			return result;
		}
		else {
			return false;
		}
	}

	/**
	*  指定ボタンが押されているかどうか判定する関数.
	*  @param key<int> 0:↑, 1:↓, 2:→, 3:←, 4:決定キー, 5:キャンセルキー......
	*  @param mode<int> 0:押されているかどうか, 1:今押されたかどうか
	*  @return <bool> 判定結果
	*/
	bool Joystick::getPushState(int key, int mode) {
		if (JoystickEnabled) {
			bool result = false;
			if (!mode) {
				result = ButtonState[key];
			}
			else {
				if ((oldButtonState[key] == false) && (ButtonState[key] == true)) result = true;
			}
			return result;
		}
		else {
			return false;
		}
	}

	/**
	*  1つでもボタンが押されているかどうか判定する関数.
	*  @param mode<int> 0:押されているかどうか, 1:今押されたかどうか
	*  @return <bool> 判定結果
	*/
	bool Joystick::getPushStateAny(int mode) {
		if (JoystickEnabled) {
			if (!mode) {
				for (int i = 0; i < 20; i++) {
					if (ButtonState[i] == true) {
						return true;
					}
				}
				return false;
			}
			else {
				for (int i = 0; i < 20; i++) {
					if ((oldButtonState[i] == false) && (ButtonState[i] == true)) {
						return true;
					}
				}
				return false;
			}
		}
		else {
			return false;
		}
	}
	
	/**
	* キー割り当てをセットしData/conf.iniに書き込む関数.
	*/
	void Joystick::setButtonNum() {

	}
	
	/**
	* キー割り当てをData/conf.iniから読み込む関数.
	*/
	void Joystick::loadButtonNum() {
		TCHAR* configFilePath = _T("Data/conf.ini");
		ButtonNum[0] = GetPrivateProfileInt(_T("joystick"), _T("up"), 0, configFilePath);
		ButtonNum[1] = GetPrivateProfileInt(_T("joystick"), _T("down"), 1, configFilePath);
		ButtonNum[2] = GetPrivateProfileInt(_T("joystick"), _T("right"), 2, configFilePath);
		ButtonNum[3] = GetPrivateProfileInt(_T("joystick"), _T("left"), 3, configFilePath);
		ButtonNum[4] = GetPrivateProfileInt(_T("joystick"), _T("btn_right"), 1, configFilePath) + 3;
		ButtonNum[5] = GetPrivateProfileInt(_T("joystick"), _T("btn_down"), 2, configFilePath) + 3;
		ButtonNum[6] = GetPrivateProfileInt(_T("joystick"), _T("btn_up"), 3, configFilePath) + 3;
		ButtonNum[7] = GetPrivateProfileInt(_T("joystick"), _T("btn_left"), 4, configFilePath) + 3;
		ButtonNum[8] = GetPrivateProfileInt(_T("joystick"), _T("btn_R"), 5, configFilePath) + 3;
		ButtonNum[9] = GetPrivateProfileInt(_T("joystick"), _T("btn_L"), 6, configFilePath) + 3;
		ButtonNum[10] = GetPrivateProfileInt(_T("joystick"), _T("btn_select"), 7, configFilePath) + 3;
		ButtonNum[11] = GetPrivateProfileInt(_T("joystick"), _T("btn_start"), 8, configFilePath) + 3;
	}

	/**
	*  入力デバイスを再開する関数.
	*  @return <HRESULT> デバイスが有効ならAcquire関数の戻り値, 無効ならE_FAIL
	*/
	HRESULT Joystick::acquire() {
		if (JoystickEnabled) {
			return DirectInputDevice8->Acquire();
		}
		else {
			return E_FAIL;
		}
	}

	BOOL CALLBACK Joystick::EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext) {
		if (FAILED(DirectInput8->CreateDevice(pdidInstance->guidInstance, &DirectInputDevice8, NULL))) {
			return DIENUM_CONTINUE;
		}
		else {
			return DIENUM_STOP;
		}
	}

	BOOL CALLBACK Joystick::EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext) {
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_BYID;
		diprg.diph.dwObj = pdidoi->dwType;
		diprg.lMin = 0 - 1000;
		diprg.lMax = 0 + 1000;

		if (FAILED(DirectInputDevice8->SetProperty(DIPROP_RANGE, &diprg.diph))) {
			return DIENUM_STOP;
		}
		else {
			return DIENUM_CONTINUE;
		}
	}
	
	/**
	*  コンストラクタ
	*  @param hWnd<HWND> ウィンドウハンドル
	*/
	InputDevice::InputDevice(HWND hWnd) :
			joystick        (hWnd),
			keyboard        (hWnd) {

	}

	/**
	*  デストラクタ
	*/
	InputDevice::~InputDevice() {

	}

	/**
	*  入力デバイスの少なくともどちらかが有効であるかを判定する関数.
	*  @return <bool> 判定結果
	*/
	bool InputDevice::getEnabled() {
		return joystick.getEnabled() || keyboard.getEnabled();
	}

	/**
	*  全ボタンの押下状態を保存する関数.
	*  @return <bool> 正常に保存できればtrue, そうでなければfalse
	*/
	bool InputDevice::getPushState() {
		bool joy = joystick.getPushState();
		bool key = keyboard.getPushState();
		return joy || key;
	}
	
	/**
	*  指定ボタンが押されているかどうか判定する関数.
	*  @param key<int> 0:↑, 1:↓, 2:→, 3:←, 4:決定キー, 5:キャンセルキー......
	*  @param mode<int> 0:押されているかどうか, 1:今押されたかどうか
	*  @return <bool> 判定結果
	*/
	bool InputDevice::getPushState(int key, int mode) {
		return joystick.getPushState(key, mode) || keyboard.getPushState(keyboardKey[key], mode);
	}
	
	/**
	*  1つでもボタンが押されているかどうか判定する関数.
	*  @param mode<int> 0:押されているかどうか, 1:今押されたかどうか
	*  @return <bool> 判定結果
	*/
	bool InputDevice::getPushStateAny(int mode) {
		return joystick.getPushStateAny(mode) || keyboard.getPushStateAny(mode);
	}
	
	/**
	*  入力デバイスを再開する関数.
	*  @return <bool> どちらかのデバイスが再開できればtrue
	*/
	bool InputDevice::acquire() {
		bool joy = (bool)SUCCEEDED(joystick.acquire());
		bool key = (bool)SUCCEEDED(keyboard.acquire());
		return joy || key;
	}
}