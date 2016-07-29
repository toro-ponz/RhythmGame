#define	DIRECTINPUT_VERSION 0x0800

#include "KeyboardClass.h"

namespace Device {
    //�R���X�g���N�^
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

    //�f�X�g���N�^
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

	//getEnabled�֐��@�@�@�@�@�@�@�@�@�@�@�L�[�{�[�h�̏��(Enabled or Disabled)���擾����
	//���� 1�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�Ȃ�
	//�߂�l�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@keyboardEnabled(true or false)
	bool Keyboard::getEnabled() {
		return keyboardEnabled;
	}

	//getPushState�֐��@�@�@�@�@�@�@�@�@�@�L�[�̉�����Ԃ��擾�A�ۑ�
	//���� 1�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�Ȃ�
	//�߂�l�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@���s�����false
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

	//getPushState�֐��@�@�@�@�@�@�@�@�@�@�w��L�[��������Ă��邩�ǂ�����Ԃ�
	//���� 1�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@DXINPUT�̃L�[�l
	//���� 2�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@���[�h
	//�߂�l�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@true or false
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

	//getPushStateAny�֐� �@�@�@�@�@�@�@�@�L�[��������Ă��邩�ǂ�����Ԃ�
	//���� 1�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@���[�h
	//�߂�l�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@true or false
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

	//acquire�֐� �@�@�@�@�@�@�@        �@�f�o�C�X�̍ĊJ
	//���� 1�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�Ȃ�
	//�߂�l�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@HRESULT
	HRESULT Keyboard::acquire() {
		return g_pDIDevice->Acquire();
	}
}