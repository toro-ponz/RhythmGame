#include "InputDeviceClass.h"

namespace Device {
	//�R���X�g���N�^
	InputDevice::InputDevice(HWND hWnd) {
		joystick = new Joystick(hWnd);
		keyboard = new Keyboard(hWnd);
		joystickEnabled = joystick->getEnabled();
		keyboardEnabled = keyboard->getEnabled();
	}

	//�f�X�g���N�^
	InputDevice::~InputDevice() {
		delete joystick;
		delete keyboard;
	}

	//deviceEnabled�֐�                         ���̓f�o�C�X�̏��Ȃ��Ƃ��ǂ��炩���L���ł��邩�𔻒肷��
	//����                                      �Ȃ�
	//�߂�l                                    ���̓f�o�C�X�̏��(true or false)
	bool InputDevice::getEnabled() {
		return joystick->getEnabled() || keyboard->getEnabled();
	}

	//getPushState�֐�                          �S�L�[�̓��͏�Ԃ��擾�A�i�[
	//����                                      �Ȃ�
	//�߂�l                                    �����A���s(true or false)
	bool InputDevice::getPushState() {
		bool joy = joystick->getPushState();
		bool key = keyboard->getPushState();
		return joy || key;
	}

	//getPushState�֐�                          �w��L�[��������Ă��邩(���͉����ꂽ��)�𔻒肷��
	//���� 1                                    �L�[�ԍ�
	//���� 2                                    ���[�h(0:������Ă��邩�ǂ���, 1:�����ꂽ���ǂ���)
	//�߂�l                                    �w��L�[�̓��͏��(true or false)
	bool InputDevice::getPushState(int key, int mode) {
		return joystick->getPushState(key, mode) || keyboard->getPushState(keyboardKey[key], mode);
	}

	//getPushStateAny�֐�                       ��ł��L�[��������Ă��邩(���͉����ꂽ��)�𔻒肷��
	//����                                      ���[�h(0:������Ă��邩�ǂ���, 1:�����ꂽ���ǂ���)
	//�߂�l                                    ���̓f�o�C�X�̏��(true or false)
	bool InputDevice::getPushStateAny(int mode) {
		return joystick->getPushStateAny(mode) || keyboard->getPushStateAny(mode);
	}
	
	//acquire�֐�                               �f�o�C�X�̍ĊJ
	//����                                      �Ȃ�
	//�߂�l                                    HRESULT
	HRESULT InputDevice::acquire() {
		return SUCCEEDED(joystick->acquire()) || SUCCEEDED(keyboard->acquire());
	}
}