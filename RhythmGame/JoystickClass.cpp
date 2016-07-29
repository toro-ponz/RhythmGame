#define	DIRECTINPUT_VERSION 0x0800

#include "JoystickClass.h"

namespace Device {
	//コンストラクタ
	Joystick::Joystick(HWND hWnd) {
		HINSTANCE hInst = GetModuleHandle(NULL);
		//DirectInput8インターフェイスの取得
		if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&lpDI, NULL))) {
			JoystickEnabled = false;
		}
		else {
			lpDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY);
			if (lpDIDevice != NULL) {
				// データフォーマットの設定
				lpDIDevice->SetDataFormat(&c_dfDIJoystick);
				// 協調モードの設定
				lpDIDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
				diDevCaps.dwSize = sizeof(DIDEVCAPS);
				lpDIDevice->GetCapabilities(&diDevCaps);
				lpDIDevice->EnumObjects(EnumAxesCallback, (VOID*)hWnd, DIDFT_AXIS);
				// 入力の開始
				lpDIDevice->Poll();
				lpDIDevice->Acquire();
				JoystickEnabled = true;
			}
			else {
				JoystickEnabled = false;
			}
		}
		ButtonNum[0] = GetPrivateProfileInt(_T("joystick"), _T("up"), 0, _T("Data/conf.ini"));
		ButtonNum[1] = GetPrivateProfileInt(_T("joystick"), _T("down"), 1, _T("Data/conf.ini"));
		ButtonNum[2] = GetPrivateProfileInt(_T("joystick"), _T("right"), 2, _T("Data/conf.ini"));
		ButtonNum[3] = GetPrivateProfileInt(_T("joystick"), _T("left"), 3, _T("Data/conf.ini"));
		ButtonNum[4] = GetPrivateProfileInt(_T("joystick"), _T("btn_right"), 1, _T("Data/conf.ini")) + 3;
		ButtonNum[5] = GetPrivateProfileInt(_T("joystick"), _T("btn_down"), 2, _T("Data/conf.ini")) + 3;
		ButtonNum[6] = GetPrivateProfileInt(_T("joystick"), _T("btn_up"), 3, _T("Data/conf.ini")) + 3;
		ButtonNum[7] = GetPrivateProfileInt(_T("joystick"), _T("btn_left"), 4, _T("Data/conf.ini")) + 3;
		ButtonNum[8] = GetPrivateProfileInt(_T("joystick"), _T("btn_R"), 5, _T("Data/conf.ini")) + 3;
		ButtonNum[9] = GetPrivateProfileInt(_T("joystick"), _T("btn_L"), 6, _T("Data/conf.ini")) + 3;
		ButtonNum[10] = GetPrivateProfileInt(_T("joystick"), _T("btn_select"), 7, _T("Data/conf.ini")) + 3;
		ButtonNum[11] = GetPrivateProfileInt(_T("joystick"), _T("btn_start"), 8, _T("Data/conf.ini")) + 3;
	}

	//デストラクタ
	Joystick::~Joystick() {
		if (JoystickEnabled) {
			lpDIDevice->Unacquire();
		}
		if (lpDIDevice != NULL) lpDIDevice->Release();
		if (lpDI != NULL) lpDI->Release();
	}

	//getJoystickEnabled関数　　　　　　　ジョイスティックが接続されており、正しく動作するか判定する
	//引数　　　　　　　　　　　　　　　　なし
	//戻り値　　　　　　　　　　　　　　　ジョイスティックの状態(true or false)
	bool Joystick::getEnabled() {
		return JoystickEnabled;
	}

	//getPushState関数　　　　　　　　　　全ボタン押下を判定、保存する
	//引数 1　　　　　　　　　　　　　　　なし
	//戻り値　　　　　　　　　　　　　　　成功、失敗(true or false)
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

	//getPushStateAny関数 　　　　　　　　１つでもボタンが押されているかどうか
	//引数 1　　　　　　　　　　　　　　　0:押されているかどうか, 1:今押されたかどうか
	//戻り値　　　　　　　　　　　　　　　true or false
	bool Joystick::getPushStateAny(int i) {
		if (JoystickEnabled) {
			if (i == 0) {
				for (int i = 0; i < 20; i++) {
					if (ButtonState[i] == true) return true;
				}
				return false;
			}
			else {
				for (int i = 0; i < 20; i++) {
					if ((oldButtonState[i] == false) && (ButtonState[i] == true)) return true;
				}
				return false;
			}
		}
		else {
			return false;
		}
	}

	//getPushState関数　　　　　　　　　　ボタン判定を取り出す
	//引数 1　　　　　　　　　　　　　　　0:↑, 1:↓, 2:→, 3:←, 4:決定キー, 5:キャンセルキー......
	//戻り値　　　　　　　　　　　　　　　指定ボタンの状態(true or false)
	bool Joystick::getPushState(int i) {
		if (JoystickEnabled) {
			bool result = false;
			HRESULT hr = lpDIDevice->GetDeviceState(sizeof(DIJOYSTATE), &js);
			if (i == 0) {
				if (js.lY == -1000) result = true;
			}
			else if (i == 1) {
				if (js.lY == 1000) result = true;
			}
			else if (i == 2) {
				if (js.lX == 1000) result = true;
			}
			else if (i == 3) {
				if (js.lX == -1000) result = true;
			}
			else {
				if (js.rgbButtons[ButtonNum[i] - 4] & 0x80) result = true;
			}
			return result;
		}
		else {
			return false;
		}
	}

	//getPushState関数　　　　　　　　　　ボタン判定を取り出す
	//引数 1　　　　　　　　　　　　　　　0:↑, 1:↓, 2:→, 3:←, 4:決定キー, 5:キャンセルキー......
	//引数 2　　　　　　　　　　　　　　　0:押されているかどうか, 1:今押されたかどうか
	//戻り値　　　　　　　　　　　　　　　指定ボタンの状態(true or false)
	bool Joystick::getPushState(int i, int j) {
		if (JoystickEnabled) {
			bool result = false;
			if (j == 0) {
				result = ButtonState[i];
			}
			else if (j == 1) {
				if ((oldButtonState[i] == false) && (ButtonState[i] == true)) result = true;
			}
			return result;
		}
		else {
			return false;
		}
	}

	//setButtonNum関数　　　　　　　　　　ボタン設定の適用
	//引数 1　　　　　　　　　　　　　　　なし
	//戻り値　　　　　　　　　　　　　　　なし
	void Joystick::setButtonNum() {

	}

	//Acquire関数　　　　　　　　　　　　 デバイスの再開
	//引数　　　　　　　　　　　　　　　　なし
	//戻り値　　　　　　　　　　　　　　　HRESULT
	HRESULT Joystick::acquire() {
		if (JoystickEnabled) {
			return lpDIDevice->Acquire();
		}
		else {
			return E_FAIL;
		}
	}

	BOOL CALLBACK Joystick::EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext) {
		HRESULT hr;
		hr = lpDI->CreateDevice(pdidInstance->guidInstance, &lpDIDevice, NULL);
		if (FAILED(hr)) return DIENUM_CONTINUE;
		return DIENUM_STOP;
	}

	BOOL CALLBACK Joystick::EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext) {
		DIPROPRANGE diprg;

		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_BYID;
		diprg.diph.dwObj = pdidoi->dwType;
		diprg.lMin = 0 - 1000;
		diprg.lMax = 0 + 1000;

		if (FAILED(lpDIDevice->SetProperty(DIPROP_RANGE, &diprg.diph))) return DIENUM_STOP;

		return DIENUM_CONTINUE;
	}
}