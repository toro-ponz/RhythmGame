#define	DIRECTINPUT_VERSION 0x0800

#include "KeyboardClass.h"

namespace Device {
    //コンストラクタ
    Keyboard::Keyboard(HWND hWnd) {
        if (SUCCEEDED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pDInput, NULL))) {
            if (SUCCEEDED(g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevice, NULL))) {
				if (SUCCEEDED(g_pDIDevice->SetDataFormat(&c_dfDIKeyboard))) {
					if (SUCCEEDED(g_pDIDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))) {
						g_pDIDevice->Acquire();
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
            else {
                keyboardEnabled = false;
            }
        }
        else {
            keyboardEnabled = false;
        }
    }

    //デストラクタ
    Keyboard::~Keyboard() {
		g_pDIDevice->Unacquire();
		if (g_pDIDevice != NULL) {
			g_pDIDevice->Release();
			g_pDIDevice = NULL;
		}
		if (g_pDInput != NULL) {
			g_pDInput->Release();
			g_pDInput = NULL;
		}
    }

	//getEnabled関数　　　　　　　　　　　キーボードの状態(Enabled or Disabled)を取得する
	//引数 1　　　　　　　　　　　　　　　なし
	//戻り値　　　　　　　　　　　　　　　keyboardEnabled(true or false)
	bool Keyboard::getEnabled() {
		return keyboardEnabled;
	}

	//getPushState関数　　　　　　　　　　キーの押下状態を取得、保存
	//引数 1　　　　　　　　　　　　　　　なし
	//戻り値　　　　　　　　　　　　　　　失敗すればfalse
	bool Keyboard::getPushState() {
		memcpy(oldKeyState, keyState, sizeof(keyState));
		if (keyboardEnabled) {
			if (SUCCEEDED(g_pDIDevice->GetDeviceState(sizeof(keyState), (LPVOID)&keyState))) {
				return true;
			}
			else {
				g_pDIDevice->Acquire();
				if (SUCCEEDED(g_pDIDevice->GetDeviceState(sizeof(keyState), (LPVOID)&keyState))) {
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

	//getPushState関数　　　　　　　　　　指定キーが押されているかどうかを返す
	//引数 1　　　　　　　　　　　　　　　DXINPUTのキー値
	//引数 2　　　　　　　　　　　　　　　モード
	//戻り値　　　　　　　　　　　　　　　true or false
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

	//getPushStateAny関数 　　　　　　　　キーが押されているかどうかを返す
	//引数 1　　　　　　　　　　　　　　　モード
	//戻り値　　　　　　　　　　　　　　　true or false
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

	//acquire関数 　　　　　　　        　デバイスの再開
	//引数 1　　　　　　　　　　　　　　　なし
	//戻り値　　　　　　　　　　　　　　　HRESULT
	HRESULT Keyboard::acquire() {
		return g_pDIDevice->Acquire();
	}
}