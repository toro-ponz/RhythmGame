#include "InputDevice.h"

namespace Device {
	/**
	*  �R���X�g���N�^
	*  @param hWnd<HWND> �E�B���h�E�n���h��
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
	*  �f�X�g���N�^
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
	*  �L�[�{�[�h���ڑ�����Ă���A���������삷�邩���肷��֐�.
	*  @return <bool> keyboardEnabled
	*/
	bool Keyboard::getEnabled() {
		return keyboardEnabled;
	}

	/**
	*  �S�{�^���̉�����Ԃ�ۑ�����֐�.
	*  @return <bool> ����ɕۑ��ł����true, �����łȂ����false
	*/
	bool Keyboard::getPushState() {
		memcpy(oldKeyState, currentKeyState, sizeof(currentKeyState));
		if (keyboardEnabled) {
			if (SUCCEEDED(DirectInputDevice8->GetDeviceState(sizeof(currentKeyState), (LPVOID)&currentKeyState))) {
				return true;
			}
			else {
				DirectInputDevice8->Acquire();
				if (SUCCEEDED(DirectInputDevice8->GetDeviceState(sizeof(currentKeyState), (LPVOID)&currentKeyState))) {
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
	*  �w��{�^����������Ă��邩�ǂ������肷��֐�.
	*  @param key<int> �L�[�ԍ�
	*  @param mode<int> 0:������Ă��邩�ǂ���, 1:�������ꂽ���ǂ���
	*  @return <bool> ���茋��
	*/
	bool Keyboard::getPushState(int key, int mode) {
		if (keyboardEnabled) {
			if (!mode) {
				if (currentKeyState[key] & 0x80) {
					return true;
				}
				else {
					return false;
				}
			}
			else {
				if ((currentKeyState[key] & 0x80) && !(oldKeyState[key] & 0x80)) {
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
	*  1�ł��{�^����������Ă��邩�ǂ������肷��֐�.
	*  @param mode<int> 0:������Ă��邩�ǂ���, 1:�������ꂽ���ǂ���
	*  @return <bool> ���茋��
	*/
	bool Keyboard::getPushStateAny(int mode) {
		if (keyboardEnabled) {
			if (!mode) {
				for (int i = 0; i < sizeof(currentKeyState); i++) {
					if (currentKeyState[i] & 0x80) {
						return true;
					}
				}
				return false;
			}
			else {
				for (int i = 0; i < sizeof(currentKeyState); i++) {
					if ((currentKeyState[i] & 0x80) && !(oldKeyState[i] & 0x80)) {
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
	*  ���̓f�o�C�X���ĊJ����֐�.
	*  @return <HRESULT> �f�o�C�X���L���Ȃ�Acquire�֐��̖߂�l, �����Ȃ�E_FAIL
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
	*  �R���X�g���N�^
	*  @param hWnd<HWND> �E�B���h�E�n���h��
	*/
	Joystick::Joystick(HWND hWnd) {
		HINSTANCE hInst = GetModuleHandle(NULL);
		//DirectInput8�C���^�[�t�F�C�X�̎擾
		if (SUCCEEDED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DirectInput8, NULL))) {
			if (SUCCEEDED(DirectInput8->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY))) {
				if (DirectInputDevice8 != NULL) {
					DirectInputDevice8->SetDataFormat(&c_dfDIJoystick);
					// �������[�h�̐ݒ�
					DirectInputDevice8->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
					diDevCaps.dwSize = sizeof(DIDEVCAPS);
					DirectInputDevice8->GetCapabilities(&diDevCaps);
					DirectInputDevice8->EnumObjects(EnumAxesCallback, (VOID*)hWnd, DIDFT_AXIS);
					// ���͂̊J�n
					DirectInputDevice8->Poll();
					DirectInputDevice8->Acquire();
					loadKeyNumber();
					joystickEnabled = true;
				}
				else {
					joystickEnabled = false;
				}
			}
			else {
				joystickEnabled = false;
			}
		}
		else {
			joystickEnabled = false;
		}
	}

	/**
	*  �f�X�g���N�^
	*/
	Joystick::~Joystick() {
		if (joystickEnabled) {
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
	*  �W���C�X�e�B�b�N���ڑ�����Ă���A���������삷�邩���肷��֐�.
	*  @return <bool> joystickEnabled
	*/
	bool Joystick::getEnabled() {
		return joystickEnabled;
	}

	/**
	*  �S�{�^���̉�����Ԃ�ۑ�����֐�.
	*  @return <bool> joystickEnabled
	*/
	bool Joystick::getPushState() {
		if (joystickEnabled) {
			for (int i = 0; i < 20; i++) {
				//�O��̏�Ԃ�old�ϐ��ɕۑ�����
				oldButtonState[i] = currentButtonState[i];
				//����̏�Ԃ�ϐ��ɕۑ�����
				currentButtonState[i] = getPushState(i);
			}
			return true;
		}
		else {
			return false;
		}
	}
	
	/**
	*  �w��{�^����������Ă��邩�ǂ������肷��֐�.
	*  @param key<int> �L�[�ԍ�
	*  @return <bool> ���茋��
	*/
	bool Joystick::getPushState(int key) {
		if (joystickEnabled) {
			bool result = false;
			HRESULT hr = DirectInputDevice8->GetDeviceState(sizeof(DIJOYSTATE), &js);
			if (key == 0) {
				if (js.lY == -1000) result = true;
			}
			else if (key == 1) {
				if (js.lY == 1000) result = true;
			}
			else if (key == 2) {
				if (js.lX == -1000) result = true;
			}
			else if (key == 3) {
				if (js.lX == 1000) result = true;
			}
			else {
				if (js.rgbButtons[keyNumber[key] - 4] & 0x80) result = true;
			}
			return result;
		}
		else {
			return false;
		}
	}

	/**
	*  �w��{�^����������Ă��邩�ǂ������肷��֐�.
	*  @param key<int> �L�[�ԍ�
	*  @param mode<int> 0:������Ă��邩�ǂ���, 1:�������ꂽ���ǂ���
	*  @return <bool> ���茋��
	*/
	bool Joystick::getPushState(int key, int mode) {
		if (joystickEnabled) {
			bool result = false;
			if (!mode) {
				result = currentButtonState[key];
			}
			else {
				if ((oldButtonState[key] == false) && (currentButtonState[key] == true)) result = true;
			}
			return result;
		}
		else {
			return false;
		}
	}

	/**
	*  1�ł��{�^����������Ă��邩�ǂ������肷��֐�.
	*  @param mode<int> 0:������Ă��邩�ǂ���, 1:�������ꂽ���ǂ���
	*  @return <bool> ���茋��
	*/
	bool Joystick::getPushStateAny(int mode) {
		if (joystickEnabled) {
			if (!mode) {
				for (int i = 0; i < 20; i++) {
					if (currentButtonState[i] == true) {
						return true;
					}
				}
				return false;
			}
			else {
				for (int i = 0; i < 20; i++) {
					if ((oldButtonState[i] == false) && (currentButtonState[i] == true)) {
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
	* �L�[���蓖�Ă��Z�b�g��Data/conf.ini�ɏ������ފ֐�.
	*/
	void Joystick::setKeyNumber() {

	}
	
	/**
	* �L�[���蓖�Ă�Data/conf.ini����ǂݍ��ފ֐�.
	*/
	void Joystick::loadKeyNumber() {
		TCHAR* configFilePath = _T("Data/conf.ini");
		keyNumber[0] = GetPrivateProfileInt(_T("joystick"), _T("arrow-up"), 1, configFilePath) - 1;
		keyNumber[1] = GetPrivateProfileInt(_T("joystick"), _T("arrow-down"), 2, configFilePath) - 1;
		keyNumber[2] = GetPrivateProfileInt(_T("joystick"), _T("arrow-left"), 3, configFilePath) - 1;
		keyNumber[3] = GetPrivateProfileInt(_T("joystick"), _T("arrow-right"), 4, configFilePath) - 1;
		keyNumber[4] = GetPrivateProfileInt(_T("joystick"), _T("button-up"), 1, configFilePath) + 3;
		keyNumber[5] = GetPrivateProfileInt(_T("joystick"), _T("button-down"), 2, configFilePath) + 3;
		keyNumber[6] = GetPrivateProfileInt(_T("joystick"), _T("button-left"), 3, configFilePath) + 3;
		keyNumber[7] = GetPrivateProfileInt(_T("joystick"), _T("button-right"), 4, configFilePath) + 3;
		keyNumber[8] = GetPrivateProfileInt(_T("joystick"), _T("button-select"), 5, configFilePath) + 3;
		keyNumber[9] = GetPrivateProfileInt(_T("joystick"), _T("button-start"), 6, configFilePath) + 3;
		keyNumber[10] = GetPrivateProfileInt(_T("joystick"), _T("trigger-left"), 7, configFilePath) + 3;
		keyNumber[11] = GetPrivateProfileInt(_T("joystick"), _T("trigger-right"), 8, configFilePath) + 3;
	}

	/**
	*  ���̓f�o�C�X���ĊJ����֐�.
	*  @return <HRESULT> �f�o�C�X���L���Ȃ�Acquire�֐��̖߂�l, �����Ȃ�E_FAIL
	*/
	HRESULT Joystick::acquire() {
		if (joystickEnabled) {
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
	*  �R���X�g���N�^
	*  @param hWnd<HWND> �E�B���h�E�n���h��
	*/
	InputDevice::InputDevice(HWND hWnd) :
			joystick(hWnd),
			keyboard(hWnd) {

	}

	/**
	*  �f�X�g���N�^
	*/
	InputDevice::~InputDevice() {

	}

	/**
	*  ���̓f�o�C�X�̏��Ȃ��Ƃ��ǂ��炩���L���ł��邩�𔻒肷��֐�.
	*  @return <bool> ���茋��
	*/
	bool InputDevice::getEnabled() {
		return joystick.getEnabled() || keyboard.getEnabled();
	}

	/**
	*  �S�{�^���̉�����Ԃ�ۑ�����֐�.
	*  @return <bool> ����ɕۑ��ł����true, �����łȂ����false
	*/
	bool InputDevice::getPushState() {
		bool joy = joystick.getPushState();
		bool key = keyboard.getPushState();
		return joy || key;
	}
	
	/**
	*  �w��{�^����������Ă��邩�ǂ������肷��֐�.
	*  @param key<KEY> �L�[�ԍ�
	*  @param mode<KEY_STATE> STATE_HAVE_PUSHED:������Ă��邩�ǂ���, STATE_PUSH:�������ꂽ���ǂ���
	*  @return <bool> ���茋��
	*/
	bool InputDevice::getPushState(KEY key, KEY_STATE mode) {
		bool joy = joystick.getPushState(joystickKeys[key], mode);
		bool kb;
		if (key == KEY::ARROW_UP || key == KEY::ARROW_DOWN || key == KEY::ARROW_LEFT || key == KEY::ARROW_RIGHT)
			kb = keyboard.getPushState(keyboardKeys[key], mode) || keyboard.getPushState(keyboardKeys[key + 14], mode);
		else
			kb = keyboard.getPushState(keyboardKeys[key], mode);
		return joy || kb;
	}
	
	/**
	*  1�ł��{�^����������Ă��邩�ǂ������肷��֐�.
	*  @param mode<KEY_STATE> STATE_HAVE_PUSHED:������Ă��邩�ǂ���, STATE_PUSH:�������ꂽ���ǂ���
	*  @return <bool> ���茋��
	*/
	bool InputDevice::getPushStateAny(KEY_STATE mode) {
		return joystick.getPushStateAny(mode) || keyboard.getPushStateAny(mode);
	}
	
	/**
	*  ���̓f�o�C�X���ĊJ����֐�.
	*  @return <bool> �ǂ��炩�̃f�o�C�X���ĊJ�ł����true
	*/
	bool InputDevice::acquire() {
		bool joy = (bool)SUCCEEDED(joystick.acquire());
		bool key = (bool)SUCCEEDED(keyboard.acquire());
		return joy || key;
	}
}