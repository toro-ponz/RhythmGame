#include "ConfigRenderClass.h"
#include "InputDevice.h"
#include "Global.h"
#include <shlwapi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

namespace Frame {
	/**
	*  �R���X�g���N�^
	*/
	ConfigRender::ConfigRender() :
			Background(imageDirectoryPath + "background.png"),
			Title(configImageDirectoryPath + "title.png"),
			Buttons(configImageDirectoryPath + "buttons.png", 2, 240.0f, 296.0f),
			Cursor(configImageDirectoryPath + "cursor.png", 1, 32.0f),
			SaveComplate(configImageDirectoryPath + "save-complate.png", 1),
			Frame1(configImageDirectoryPath + "frame.png", 1),
			EditCursor(configImageDirectoryPath + "edit-cursor.png", 1, 50.0f),
			Black(colorDirectoryPath + "black.png") {
		Title.setPosition(0, -20.0f, -20.0f);

		Buttons.setRectFromChip(0, 1);
		Buttons.setRectFromChip(1, 2);

		DWORD interval = 150;
		Cursor.setPosition(0, 30.0f, 300.0f);
		Cursor.setAnimationInterval(0, 1, 4, &interval, 1, 4);

		SaveComplate.setCenter(0, 255.0f, 50.0f);
		SaveComplate.setPosition(0, 512.0f, 640.0f);
		SaveComplate.disableDraw(0);

		Frame1.setCenter(0, 352.0f, 276.0f);
		Frame1.setPosition(0, 512.0f, 384.0f);
		Frame1.disableDraw(0);

		EditCursor.setPosition(0, 200.0f, 207.0f);
		EditCursor.setAnimationInterval(0, 1, 4, &interval, 1, 4);
		EditCursor.disableDraw(0);

		Black.setRectFromPixel(0, 0, 768, 0, 1024);
		Black.setPosition(0, 0.0, 0.0);
		Black.setColor(0, D3DCOLOR_ARGB(128, 255, 255, 255));
		Black.disableDraw(0);

		configText = vector<FontTextureCreate>(11);
		screenModeText = vector<FontTextureCreate>(5);
		vSyncText = vector<FontTextureCreate>(5);
		autoPlayText = vector<FontTextureCreate>(5);
		customSkinText = vector<FontTextureCreate>(5);
		timingText = vector<FontTextureCreate>(3);
		keyboardText = vector<FontTextureCreate>(3);
		joystickText = vector<FontTextureCreate>(3);

		initConfigData();
		loadConfig();
		saveConfig();

		textCreate();

		editCursorNumber = 1;
		parameterBuffer = 1;
		settingKeyboard = 0;
		settingJoystick = 0;
		editingConfig = false;
		changedKeyboardSettings = false;
		changedJoystickSettings = false;
		state = STATE::MAIN;
		mainState = MAIN_STATE::SCREEN_MODE;
	}

	/**
	*  �f�X�g���N�^
	*/
	ConfigRender::~ConfigRender() {

	}

	/**
	*  �`�悷��֐�.
	*/
	void ConfigRender::Rend() {
		if (saveComplateTextDrawTime + 2000 < timeGetTime())
			SaveComplate.disableDraw(0);

		inputDevice->getPushState();
		if (editingConfig && mainState == MAIN_STATE::KEYBOARD_SETTINGS) {
			setKeyboard();
		}
		else if (editingConfig && mainState == MAIN_STATE::JOYSTICK_SETTINGS) {
			setJoystick();
		}
		else {
			if (inputDevice->getPushState(InputDevice::KEY::ARROW_UP, InputDevice::KEY_STATE::STATE_PUSH))
				pushArrowUp();
			else if (inputDevice->getPushState(InputDevice::KEY::ARROW_DOWN, InputDevice::KEY_STATE::STATE_PUSH))
				pushArrowDown();
			else if (inputDevice->getPushState(InputDevice::KEY::ARROW_LEFT, InputDevice::KEY_STATE::STATE_PUSH))
				pushArrowLeft();
			else if (inputDevice->getPushState(InputDevice::KEY::ARROW_RIGHT, InputDevice::KEY_STATE::STATE_PUSH))
				pushArrowRight();
			else if (inputDevice->getPushState(InputDevice::KEY::BUTTON_DOWN, InputDevice::KEY_STATE::STATE_PUSH))
				pushButtonDown();
			else if (inputDevice->getPushState(InputDevice::KEY::BUTTON_RIGHT, InputDevice::KEY_STATE::STATE_PUSH))
				pushButtonRight();
			else if (inputDevice->getPushState(InputDevice::KEY::RETURN, InputDevice::KEY_STATE::STATE_PUSH))
				pushReturn();
			else if (inputDevice->getPushState(InputDevice::KEY::ESCAPE, InputDevice::KEY_STATE::STATE_PUSH))
				pushEscape();
		}
		editConfig();

		switch (state) {
		case STATE::MAIN:
			Buttons.setPosition(0, 880.0f, 190.0f);
			Buttons.setPosition(1, 880.0f, 470.0f);
			Cursor.enableDraw(0);
			break;
		case STATE::SAVE:
			Buttons.setPosition(0, 880.0f - 50.0f, 190.0f);
			Buttons.setPosition(1, 880.0f, 470.0f);
			Cursor.disableDraw(0);
			break;
		case STATE::BACK:
			Buttons.setPosition(0, 880.0f, 190.0f);
			Buttons.setPosition(1, 880.0f - 50.0f, 470.0f);
			Cursor.disableDraw(0);
			break;
		}

		const float cursorLeftX = 40.0f;
		const float cursorX[] = { cursorLeftX, cursorLeftX, cursorLeftX, cursorLeftX, cursorLeftX, cursorLeftX, cursorLeftX };
		const float cursorY[] = { 200.0f, 300.0f, 400.0f, 450.0f, 500.0f, 600.0f, 650.0f };
		Cursor.setPosition(0, cursorX[(int)mainState], cursorY[(int)mainState]);

		draw();
	}

	/**
	*  �ݒ����ҏW����֐�.
	*/
	void ConfigRender::editConfig() {
		if (editingConfig) {
			Black.enableDraw(0);
			Frame1.enableDraw(0);
			if (mainState < MAIN_STATE::TIMING) {
				EditCursor.enableDraw(0);
				switch (editCursorNumber) {
				case 1:
					EditCursor.setPosition(0, 200.0f, 207.0f);
					break;
				case 2:
					EditCursor.setPosition(0, 200.0f, 410.0f);
					break;
				}
			}
		}
		else {
			Black.disableDraw(0);
			EditCursor.disableDraw(0);
			Frame1.disableDraw(0);
			editCursorNumber = 1;
			for (int i = 0; i < 5; i++) {
				vSyncText[i].disableDraw();
				screenModeText[i].disableDraw();
				autoPlayText[i].disableDraw();
				customSkinText[i].disableDraw();
			}
			for (int i = 0; i < 3; i++) {
				timingText[i].disableDraw();
				keyboardText[i].disableDraw();
				joystickText[i].disableDraw();
			}
		}
	}

	/**
	*  �ݒ����conf.ini�ɏ������ފ֐�.
	*  @param first<bool> �����Z�[�u���ۂ�
	*  @return <bool> �������݂̐���
	*/
	bool ConfigRender::saveConfig(bool first) {
		for (int i = 0; i < (int)sectionName.size(); i++) {
			for (int j = 0; j < (int)configData[i].size(); j++) {
				WritePrivateProfileString(
					convertStringToTchar(sectionName[i]),
					convertStringToTchar(configData[i][j].key),
					convertStringToTchar(configData[i][j].value),
					convertStringToTchar(configFilePath)
				);
			}
		}
		if (!first) {
			saveComplateTextDrawTime = timeGetTime();
			SaveComplate.enableDraw(0);
			changedKeyboardSettings = false;
			changedJoystickSettings = false;
			configTextCreate(9);
			configTextCreate(10);
			for (int i = 0; i < 11; i++) {
				configText[i].setColor(D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			inputDevice->loadKeyNumber();
		}
		return true;
	}

	/**
	*  �ݒ����conf.ini����ǂݍ��ފ֐�.
	*  @return <bool> �ǂݍ��݂̐���
	*/
	bool ConfigRender::loadConfig() {
		for (int i = 0; i < (int)sectionName.size(); i++) {
			for (int j = 0; j < (int)configData[i].size(); j++) {
				TCHAR buf[99999];
				DWORD err = GetPrivateProfileString(
					convertStringToTchar(sectionName[i]),
					convertStringToTchar(configData[i][j].key),
					convertStringToTchar(configData[i][j].value),
					buf,
					sizeof(buf),
					convertStringToTchar(configFilePath)
				);
				if (err != -1) {
					changeConfig(sectionName[i], configData[i][j].key, convertTcharToString(buf));
				}
			}
		}
		return true;
	}

	/**
	*  �ݒ���(ini�`��)��ύX����֐�.
	*  @param section<string> �ύX����L�[�̑�����Z�N�V����
	*  @param key<string> �ύX����L�[
	*  @param value<string> �ύX��̒l
	*  @return <bool> �ύX�̐���
	*/
	bool ConfigRender::changeConfig(string section, string key, string value) {
		int sectionNumber = 0;
		while (true) {
			if (sectionNumber == sectionName.size())
				return false;
			if (section == sectionName[sectionNumber])
				break;
			sectionNumber++;
		}
		int i = 0;
		while (true) {
			if (i == configData[sectionNumber].size())
				return false;
			if (key == configData[sectionNumber][i].key) {
				configData[sectionNumber][i].value = value;
				break;
			}
			i++;
		}

		if (section == "window" && key == "mode") {
			configTextCreate(1);
		}
		else if (section == "directx" && key == "v-sync") {
			configTextCreate(3);
		}
		else if (section == "play") {
			if (key == "autoplay")
				configTextCreate(5);
			else if (key == "custom-skin")
				configTextCreate(6);
			else if (key == "notes-timing")
				configTextCreate(7);
		}

		return true;
	}

	/**
	*  �ݒ���̏�����������֐�.
	*  @return <bool> �������̐���
	*/
	bool ConfigRender::initConfigData() {
		configData = vector<vector<CONFIG_DATA>>(5);

		sectionName.push_back("window");
		configData[0].push_back({"mode", "WINDOW"});

		sectionName.push_back("directx");
		configData[1].push_back({"v-sync", "ON"});

		sectionName.push_back("play");
		configData[2].push_back({"autoplay", "OFF"});
		configData[2].push_back({"custom-skin", "ON"});
		configData[2].push_back({"notes-timing", "0"});

		sectionName.push_back("keyboard");
		configData[3].push_back({"arrow-up", "17"});
		configData[3].push_back({"arrow-down", "31"});
		configData[3].push_back({"arrow-left", "30"});
		configData[3].push_back({"arrow-right", "32"});
		configData[3].push_back({"button-up", "23"});
		configData[3].push_back({"button-down", "37"});
		configData[3].push_back({"button-left", "36"});
		configData[3].push_back({"button-right", "38"});
		configData[3].push_back({"button-select", "59"});
		configData[3].push_back({"button-start", "60"});
		configData[3].push_back({"trigger-left", "16"});
		configData[3].push_back({"trigger-right", "24"});
		
		sectionName.push_back("joystick");
		configData[4].push_back({"arrow-up", "1"});
		configData[4].push_back({"arrow-down", "2"});
		configData[4].push_back({"arrow-left", "3"});
		configData[4].push_back({"arrow-right", "4"});
		configData[4].push_back({"button-up", "5"});
		configData[4].push_back({"button-down", "6"});
		configData[4].push_back({"button-left", "7"});
		configData[4].push_back({"button-right", "8"});
		configData[4].push_back({"button-select", "9"});
		configData[4].push_back({"button-start", "10"});
		configData[4].push_back({"trigger-left", "11"});
		configData[4].push_back({"trigger-right", "12"});

		return true;
	}
	
	/**
	*  �ݒ��ʂ̕������p�ӂ���֐�.
	*/
	void ConfigRender::configTextCreate(int i) {
		string sm, vs, ap, cs, ti, ck, cj;
		if (configData[0][0].value == "WINDOW") sm = "�@�@�E�B���h�E";
		else sm = "�t���X�N���[��";
		if (configData[1][0].value == "ON") vs = "�@�n�m";
		else vs = "�n�e�e";
		if (configData[2][0].value == "ON") ap = "�@�n�m";
		else ap = "�n�e�e";
		if (configData[2][1].value == "ON") cs = "�@�n�m";
		else cs = "�n�e�e";

		ti = "";
		unsigned int milliSecond = std::abs(std::atoi(configData[2][2].value.c_str()));
		std::ostringstream sout;
		sout << std::setfill(' ') << std::setw(4) << milliSecond;
		std::string timingString = sout.str();
		if (std::atoi(configData[2][2].value.c_str()) >= 0)
			timingString = "+" + timingString;
		else
			timingString = "-" + timingString;
		char beforeReplacement[] = { '+', '-', ' ', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
		string afterReplacement[] = { "�{", "�|", "�@", "�O", "�P", "�Q", "�R", "�S", "�T", "�U", "�V", "�W", "�X" };
		for (int i = 0; i < (int)timingString.length(); i++) {
			for (int j = 0; j < 13; j++) {
				if (timingString[i] == beforeReplacement[j])
					ti += afterReplacement[j];
			}
		}
		if (changedKeyboardSettings)
			ck = "�ύX����Ă��܂�";
		else
			ck = "�@�@�@�@�@�@�ݒ�";
		if (changedJoystickSettings)
			cj = "�ύX����Ă��܂�";
		else
			cj = "�@�@�@�@�@�@�ݒ�";

		const string configString[] = {
			"���",
			"���[�h�@�@�@�@�@�@�@�@" + sm,
			"�`��",
			"���������@�@�@�@�@�@�@�@�@�@�@" + vs,
			"���t�E����",
			"�I�[�g�v���C�@�@�@�@�@�@�@�@�@" + ap,
			"�J�X�^���X�L���@�@�@�@�@�@�@�@" + cs,
			"�^�C�~���O�@�@�@�@�@�@" + ti + "����",
			"����",
			"�L�[�{�[�h�@�@�@�@�@" + ck,
			"�W���C�X�e�B�b�N�@�@" + cj
		};
		const float configStartX[] = { 50.0f, 100.0f, 50.0f, 100.0f, 50.0f, 100.0f, 100.0f, 100.0f, 50.0f, 100.0f, 100.0f };
		const float configStartY[] = { 150.0f, 200.0f, 250.0f, 300.0f, 350.0f, 400.0f, 450.0f, 500.0f, 550.0f, 600.0f, 650.0f };
		const int configFontSize[] = { 40, 35, 40, 35, 40, 35, 35, 35, 40, 35, 35 };
		const int configFontWeight[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
		configText[i].fontCreate(
 			configString[i],
			configStartX[i],
			configStartY[i],
			configFontSize[i],
			configFontWeight[i],
			fontName
		);
	}

	/**
	*  ��ʂ̕������p�ӂ���֐�.
	*/
	void ConfigRender::textCreate() {
		const string screenModeString[] = {
			"����ʃ��[�h",
			"�E�B���h�E",
			"�Q�[�����E�B���h�E���[�h�Ŏ��s���܂��B",
			"�t���X�N���[��",
			"�Q�[�����t���X�N���[�����[�h�Ŏ��s���܂��B"
		};
		const string vSyncString[] = {
			"����������",
			"�n�m",
			"�t���[�����[�g�̏����60�ɐ�������܂����A���肩�y���ȓ�������܂��B(����)",
			"�n�e�e",
			"�t���[�����[�g�͐������ꂸ�A���炩�ȕ`�悪���ҏo���܂����A�����ׂ��s����̉\��������܂��B"
		};
		const string autoPlayString[] = {
			"���I�[�g�v���C",
			"�n�m",
			"���ʃ��x���I�����̃I�[�g�v���C�̃f�t�H���g�l���n�m�ɂȂ�܂��B",
			"�n�e�e",
			"���ʃ��x���I�����̃I�[�g�v���C�̃f�t�H���g�l���n�e�e�ɂȂ�܂��B"
		};
		const string customSkinString[] = {
			"���J�X�^���X�L��",
			"�n�m",
			"���ʃ��x���I�����̃J�X�^���X�L���̃f�t�H���g�l���n�m�ɂȂ�܂��B",
			"�n�e�e",
			"���ʃ��x���I�����̃J�X�^���X�L���̃f�t�H���g�l���n�e�e�ɂȂ�܂��B"
		};
		const float frame1StartX[] = { 200.0f, 260.0f, 275.0f, 260.0f, 275.0f };
		const float frame1StartY[] = { 150.0f, 205.0f, 255.0f, 410.0f, 460.0f };
		const int frame1FontSize[] = { 50, 50, 35, 50, 35 };
		const int frame1FontWeight[] = { 1, 1, 1, 1, 1 };
		const float frame1Width[] = { 0.0f, 0.0f, 530.0f, 0.0f, 530.0f };

		for (int i = 0; i < 5; i++) {
			screenModeText[i].fontCreate(
				screenModeString[i],
				frame1StartX[i],
				frame1StartY[i],
				frame1FontSize[i],
				frame1FontWeight[i],
				fontName
			);
			screenModeText[i].setWidthOrHeight(frame1Width[i]);
			screenModeText[i].disableDraw();

			vSyncText[i].fontCreate(
				vSyncString[i],
				frame1StartX[i],
				frame1StartY[i],
				frame1FontSize[i],
				frame1FontWeight[i],
				fontName
			);
			vSyncText[i].setWidthOrHeight(frame1Width[i]);
			vSyncText[i].disableDraw();

			autoPlayText[i].fontCreate(
				autoPlayString[i],
				frame1StartX[i],
				frame1StartY[i],
				frame1FontSize[i],
				frame1FontWeight[i],
				fontName
			);
			autoPlayText[i].setWidthOrHeight(frame1Width[i]);
			autoPlayText[i].disableDraw();

			customSkinText[i].fontCreate(
				customSkinString[i],
				frame1StartX[i],
				frame1StartY[i],
				frame1FontSize[i],
				frame1FontWeight[i],
				fontName
			);
			customSkinText[i].setWidthOrHeight(frame1Width[i]);
			customSkinText[i].disableDraw();
		}

		for (int i = 0; i < (int)configText.size(); i++) {
			configTextCreate(i);
		}
		timingTextCreate();
		inputDeviceSettingsTextCreate();
	}

	/**
	*  �^�C�~���O������ʂ̕������p�ӂ���֐�.
	*/
	void ConfigRender::timingTextCreate() {
		const string timingString[] = {
			"���^�C�~���O����",
			"�m�[�c�̔���^�C�~���O�����炵�܂��B�{�͂��x���A�[�͂�葁���m�[�c������Ă��܂��B"
		};
		const float timingStartX[] = { 200.0f, 260.0f };
		const float timingStartY[] = { 150.0f, 205.0f };
		const int timingFontSize[] = { 50, 35 };
		const int timingFontWeight[] = { 1, 1 };
		const float timingWidth[] = { 0.0f, 530.0f };
		
		for (int i = 0; i < 2; i++) {
			timingText[i].fontCreate(
				timingString[i],
				timingStartX[i],
				timingStartY[i],
				timingFontSize[i],
				timingFontWeight[i],
				fontName
			);
			timingText[i].setWidthOrHeight(timingWidth[i]);
			timingText[i].disableDraw();
		}
		changeTimingMilliSecond(std::atoi(configData[2][2].value.c_str()));
		timingText[2].disableDraw();
	}

	/**
	*  �^�C�~���O������ʂ̐��l��ύX����֐�
	*  @param ms<int> �l
	*/
	void ConfigRender::changeTimingMilliSecond(int ms) {
		unsigned int milliSecond = std::abs(ms);
		std::ostringstream sout;
		sout << std::setfill(' ') << std::setw(4) << milliSecond;
		std::string timingString = sout.str();
		if (ms >= 0)
			timingString = "+" + timingString + "ms";
		else
			timingString = "-" + timingString + "ms";
		const float timingStartX = 420.0f;
		const float timingStartY = 430.0f;
		const int timingFontSize = 50;
		const int timingFontWeight = 1;
		
		timingText[2].fontCreate(
			timingString,
			timingStartX,
			timingStartY,
			timingFontSize,
			timingFontWeight,
			fontName
		);
	}

	/**
	*  ���̓f�o�C�X�ݒ��ʂ̕������p�ӂ���֐�.
	*/
	void ConfigRender::inputDeviceSettingsTextCreate() {
		const string keyboarString[] = {
			"���L�[�{�[�h�ݒ�",
			"�L�[�{�[�h�̃L�[���蓖�Ă�ݒ肵�܂��BESC�L�[��ENTER�L�[�A�����L�[�͊��蓖�čς݂ł��B"
		};
		const string joystickString[] = {
			"���W���C�X�e�B�b�N�ݒ�",
			"�W���C�X�e�B�b�N�̃L�[���蓖�Ă�ݒ肵�܂��B"
		};
		const float startX[] = { 200.0f, 260.0f };
		const float startY[] = { 150.0f, 205.0f };
		const int fontSize[] = { 50, 30 };
		const int fontWeight[] = { 1, 1 };
		const float width[] = { 0.0f, 530.0f };
		
		for (int i = 0; i < 2; i++) {
			keyboardText[i].fontCreate(
				keyboarString[i],
				startX[i],
				startY[i],
				fontSize[i],
				fontWeight[i],
				fontName
			);
			keyboardText[i].setWidthOrHeight(width[i]);
			keyboardText[i].disableDraw();
		}
		keyboardSettingsKeyTextCreate();
		keyboardText[2].disableDraw();
		
		for (int i = 0; i < 2; i++) {
			joystickText[i].fontCreate(
				joystickString[i],
				startX[i],
				startY[i],
				fontSize[i],
				fontWeight[i],
				fontName
			);
			joystickText[i].setWidthOrHeight(width[i]);
			joystickText[i].disableDraw();
		}
		joystickSettingsKeyTextCreate();
		joystickText[2].disableDraw();
	}
	
	/**
	*  �L�[�{�[�h�ݒ��ʂ̉����L�[�w��̕������p�ӂ���֐�.
	*  @param error<bool> �G���[�p�������p�ӂ��邩�ۂ�
	*/
	void ConfigRender::keyboardSettingsKeyTextCreate(bool error) {
		const string keyboardString[] = {
			"�ݒ���J�n����ɂ�ESC�L�[�ȊO�̃L�[�������Ă��������B�I������ɂ�ESC�L�[�������Ă��������B",
			"����ŉ�����L�[�������Ă��������B",
			"����ŉ������L�[�������Ă��������B",
			"����ŉ������L�[�������Ă��������B",
			"����ŉ����E�L�[�������Ă��������B",
			"�E��ŉ�����L�[�������Ă��������B",
			"�E��ŉ������L�[�������Ă�������(���ꂪ�L�����Z���L�[�ɂȂ�܂�)�BESC�L�[���L�����Z���Ƃقړ��l�̓�������܂��B",
			"�E��ŉ������L�[�������Ă��������B",
			"�E��ŉ����E�L�[�������Ă�������(���ꂪ����L�[�ɂȂ�܂�)�BENTER�L�[������Ƃقړ��l�̓�������܂��B",
			"�Z���N�g�{�^���������Ă��������B",
			"�X�^�[�g�{�^���������Ă��������B",
			"����ŉ�������L�[�������Ă��������B",
			"�E��ŉ����E��L�[�������Ă��������B",
			"�ݒ�͈ȏ�ŏI���ł��BENTER�L�[�������ΕύX���ۑ�����܂��BESC�L�[�������ΕύX���j������܂��B",
			"�����L�[�͐ݒ�ł��܂���B"
		};
		const float startX = 260.0f;
		const float startY = 350.0f;
		const int fontSize = 30;
		const int fontWeight = 1;
		const float width = 530.0f;
		
		if (!error) {
			keyboardText[2].fontCreate(
				keyboardString[settingKeyboard],
				startX,
				startY,
				fontSize,
				fontWeight,
				fontName
			);
			keyboardText[2].setColor(D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		else {
			keyboardText[2].fontCreate(
				keyboardString[settingKeyboard] + keyboardString[14],
				startX,
				startY,
				fontSize,
				fontWeight,
				fontName
			);
			keyboardText[2].setColor(D3DCOLOR_ARGB(255, 255, 0, 0));
		}
		keyboardText[2].setWidthOrHeight(width);
	}

	/**
	*  �W���C�X�e�B�b�N�ݒ��ʂ̉����L�[�w��̕������p�ӂ���֐�.
	*  @param error<bool> �G���[�p�������p�ӂ��邩�ۂ�
	*/
	void ConfigRender::joystickSettingsKeyTextCreate(bool error) {
		const string joystickString[] = {
			"�ݒ���J�n����ɂ�ESC�L�[�ȊO�̃L�[�������Ă��������B�I������ɂ�ESC�L�[�������Ă��������B",
			"�\���L�[�̏�������Ă��������B",
			"�\���L�[�̉��������Ă��������B",
			"�\���L�[�̍��������Ă��������B",
			"�\���L�[�̉E�������Ă�������",
			"�S�{�^��(�E��e�w�ŉ����{�^���Q)�̓��A�㑤�ɂ���{�^���������Ă��������B",
			"�S�{�^��(�E��e�w�ŉ����{�^���Q)�̓��A�����ɂ���{�^���������Ă�������(���ꂪ�L�����Z���L�[�ɂȂ�܂�)�B",
			"�S�{�^��(�E��e�w�ŉ����{�^���Q)�̓��A�����ɂ���{�^���������Ă��������B",
			"�S�{�^��(�E��e�w�ŉ����{�^���Q)�̓��A�E���ɂ���{�^���������Ă�������(���ꂪ����L�[�ɂȂ�܂�)�B",
			"�Z���N�g�{�^���������Ă��������B",
			"�X�^�[�g�{�^���������Ă��������B",
			"���g���K�[�������Ă��������B",
			"�E�g���K�[�������Ă��������B",
			"�ݒ�͈ȏ�ŏI���ł��BENTER�L�[�������ΕύX���ۑ�����܂��BESC�L�[�������ΕύX���j������܂��B",
			"�����L�[�͐ݒ�ł��܂���B"
		};
		const float startX = 260.0f;
		const float startY = 350.0f;
		const int fontSize = 30;
		const int fontWeight = 1;
		const float width = 530.0f;

		if (!error) {
			joystickText[2].fontCreate(
				joystickString[settingJoystick],
				startX,
				startY,
				fontSize,
				fontWeight,
				fontName
			);
			joystickText[2].setColor(D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		else {
			joystickText[2].fontCreate(
				joystickString[settingJoystick] + joystickString[14],
				startX,
				startY,
				fontSize,
				fontWeight,
				fontName
			);
			joystickText[2].setColor(D3DCOLOR_ARGB(255, 255, 0, 0));
		}
		joystickText[2].setWidthOrHeight(width);
	}

	/**
	*  �L�[�{�[�h�̃L�[�ݒ�֐�.
	*/
	void ConfigRender::setKeyboard() {
		if (!settingKeyboard) {
			if (editingConfig) {
				if (inputDevice->getPushStateAny(InputDevice::KEY_STATE::STATE_PUSH) &&
					!inputDevice->getPushState(InputDevice::KEY::ESCAPE, InputDevice::KEY_STATE::STATE_PUSH)) {
					switch (mainState) {
					case MAIN_STATE::KEYBOARD_SETTINGS:
						playSoundEffectFromResource(6);
						settingKeyboard = 1;
						keyboardSettingsKeyTextCreate();
						break;
					}
				}
			}
		}
		else {
			if (settingKeyboard < 13) {
				if (inputDevice->getKeyboardPushStateAny(InputDevice::KEY_STATE::STATE_PUSH)) {
					if (!inputDevice->getPushState(InputDevice::KEY::ESCAPE, InputDevice::KEY_STATE::STATE_PUSH)) {
						inputDeviceParameterBuffer[settingKeyboard - 1] = inputDevice->getKeyboardPushKeyNumber(InputDevice::KEY_STATE::STATE_PUSH);
						bool collision = false;
						for (int i = 0; i < settingKeyboard - 1; i++) {
							if (inputDeviceParameterBuffer[settingKeyboard - 1] == inputDeviceParameterBuffer[i])
								collision = true;
						}
						if (inputDeviceParameterBuffer[settingKeyboard - 1] == DIK_RETURN ||
							inputDeviceParameterBuffer[settingKeyboard - 1] == DIK_ESCAPE ||
							inputDeviceParameterBuffer[settingKeyboard - 1] == DIK_UP ||
							inputDeviceParameterBuffer[settingKeyboard - 1] == DIK_DOWN ||
							inputDeviceParameterBuffer[settingKeyboard - 1] == DIK_LEFT ||
							inputDeviceParameterBuffer[settingKeyboard - 1] == DIK_RIGHT)
							collision = true;
						if (collision) {
							playSoundEffectFromResource(8);
							keyboardSettingsKeyTextCreate(true);
						}
						else {
							playSoundEffectFromResource(5);
							settingKeyboard++;
							keyboardSettingsKeyTextCreate();
						}
					}
				}
			}
			else if (inputDevice->getPushState(InputDevice::KEY::RETURN, InputDevice::KEY_STATE::STATE_PUSH)) {
				playSoundEffectFromResource(6);
				saveKeyboard();
				settingKeyboard = 0;
				keyboardSettingsKeyTextCreate();
				editingConfig = false;
			}
		}
		if (inputDevice->getPushState(InputDevice::KEY::ESCAPE, InputDevice::KEY_STATE::STATE_PUSH)) {
			playSoundEffectFromResource(7);
			settingKeyboard = 0;
			keyboardSettingsKeyTextCreate();
			editingConfig = false;
		}
	}

	/**
	*  �W���C�X�e�B�b�N�̃L�[�ݒ�֐�.
	*/
	void ConfigRender::setJoystick() {
		if (!settingJoystick) {
			if (editingConfig) {
				if (inputDevice->getPushStateAny(InputDevice::KEY_STATE::STATE_PUSH) &&
					!inputDevice->getPushState(InputDevice::KEY::ESCAPE, InputDevice::KEY_STATE::STATE_PUSH)) {
					switch (mainState) {
					case MAIN_STATE::JOYSTICK_SETTINGS:
						playSoundEffectFromResource(6);
						settingJoystick = 1;
						joystickSettingsKeyTextCreate();
						break;
					}
				}
			}
		}
		else {
			if (settingJoystick < 13) {
				if (inputDevice->getJoystickPushStateAny(InputDevice::KEY_STATE::STATE_PUSH)) {
					inputDeviceParameterBuffer[settingJoystick - 1] = inputDevice->getJoystickPushKeyNumber(InputDevice::KEY_STATE::STATE_PUSH);
					bool collision = false;
					for (int i = 0; i < settingJoystick - 1; i++) {
						if (inputDeviceParameterBuffer[settingJoystick - 1] == inputDeviceParameterBuffer[i])
							collision = true;
					}
					if (collision) {
						playSoundEffectFromResource(8);
						joystickSettingsKeyTextCreate(true);
					}
					else {
						playSoundEffectFromResource(5);
						settingJoystick++;
						joystickSettingsKeyTextCreate();
					}
				}
			}
			else if (inputDevice->getPushState(InputDevice::KEY::RETURN, InputDevice::KEY_STATE::STATE_PUSH)) {
				playSoundEffectFromResource(6);
				saveJoystick();
				settingJoystick = 0;
				joystickSettingsKeyTextCreate();
				editingConfig = false;
			}
		}
		if (inputDevice->getPushState(InputDevice::KEY::ESCAPE, InputDevice::KEY_STATE::STATE_PUSH)) {
			playSoundEffectFromResource(7);
			settingJoystick = 0;
			joystickSettingsKeyTextCreate();
			editingConfig = false;
		}
	}

	/**
	*  �L�[�{�[�h�̃L�[�ݒ��ۑ�����֐�.
	*/
	void ConfigRender::saveKeyboard() {
		for (int i = 0; i < (int)configData[3].size(); i++) {
			configData[3][i].value = std::to_string(inputDeviceParameterBuffer[i]);
		}
		changedKeyboardSettings = true;
		configTextCreate(9);
		configText[9].setColor(D3DCOLOR_ARGB(255, 255, 0, 0));
	}

	/**
	*  �W���C�X�e�B�b�N�̃L�[�ݒ��ۑ�����֐�.
	*/
	void ConfigRender::saveJoystick() {
		for (int i = 0; i < (int)configData[4].size(); i++) {
			configData[4][i].value = std::to_string(inputDeviceParameterBuffer[i]);
		}
		changedJoystickSettings = true;
		configTextCreate(10);
		configText[10].setColor(D3DCOLOR_ARGB(255, 255, 0, 0));
	}

	/**
	*  ��L�[���������Ƃ��̏����֐�.
	*/
	void ConfigRender::pushArrowUp() {
		switch (state) {
		case STATE::MAIN:
			if (editingConfig) {
				playSoundEffectFromResource(4);
				if (mainState == MAIN_STATE::TIMING && parameterBuffer < 100) {
					parameterBuffer += 1;
					changeTimingMilliSecond(parameterBuffer);
				}
				else {
					editCursorNumber = 1;
				}
			}
			else {
				if (mainState != MAIN_STATE::SCREEN_MODE)
					playSoundEffectFromResource(4);
				switch (mainState) {
				case MAIN_STATE::V_SYNC:
					mainState = MAIN_STATE::SCREEN_MODE;
					break;
				case MAIN_STATE::AUTO_PLAY:
					mainState = MAIN_STATE::V_SYNC;
					break;
				case MAIN_STATE::CUSTOM_SKIN:
					mainState = MAIN_STATE::AUTO_PLAY;
					break;
				case MAIN_STATE::TIMING:
					mainState = MAIN_STATE::CUSTOM_SKIN;
					break;
				case MAIN_STATE::KEYBOARD_SETTINGS:
					mainState = MAIN_STATE::TIMING;
					break;
				case MAIN_STATE::JOYSTICK_SETTINGS:
					mainState = MAIN_STATE::KEYBOARD_SETTINGS;
					break;
				}
			}
			break;
		case STATE::BACK:
			playSoundEffectFromResource(4);
			state = STATE::SAVE;
		}
	}

	/**
	*  ���L�[���������Ƃ��̏����֐�.
	*/
	void ConfigRender::pushArrowDown() {
		switch (state) {
		case STATE::MAIN:
			if (editingConfig) {
				playSoundEffectFromResource(4);
				if (mainState == MAIN_STATE::TIMING && parameterBuffer > -100) {
					parameterBuffer -= 1;
					changeTimingMilliSecond(parameterBuffer);
				}
				else {
					editCursorNumber = 2;
				}
			}
			else {
				if (mainState != MAIN_STATE::JOYSTICK_SETTINGS)
					playSoundEffectFromResource(4);
				switch (mainState) {
				case MAIN_STATE::SCREEN_MODE:
					mainState = MAIN_STATE::V_SYNC;
					break;
				case MAIN_STATE::V_SYNC:
					mainState = MAIN_STATE::AUTO_PLAY;
					break;
				case MAIN_STATE::AUTO_PLAY:
					mainState = MAIN_STATE::CUSTOM_SKIN;
					break;
				case MAIN_STATE::CUSTOM_SKIN:
					mainState = MAIN_STATE::TIMING;
					break;
				case MAIN_STATE::TIMING:
					mainState = MAIN_STATE::KEYBOARD_SETTINGS;
					break;
				case MAIN_STATE::KEYBOARD_SETTINGS:
					mainState = MAIN_STATE::JOYSTICK_SETTINGS;
					break;
				}
			}
			break;
		case STATE::SAVE:
			playSoundEffectFromResource(4);
			state = STATE::BACK;
		}
	}

	/**
	*  ���L�[���������Ƃ��̏����֐�.
	*/
	void ConfigRender::pushArrowLeft() {
		switch (state) {
		case STATE::SAVE:
		case STATE::BACK:
			playSoundEffectFromResource(4);
			state = STATE::MAIN;
			break;
		}
	}

	/**
	*  �E�L�[���������Ƃ��̏����֐�.
	*/
	void ConfigRender::pushArrowRight() {
		switch (state) {
		case STATE::MAIN:
			if (!editingConfig) {
				playSoundEffectFromResource(4);
				state = STATE::SAVE;
			}
			break;
		}
	}

	/**
	*  ���{�^��(�L�����Z���L�[)���������Ƃ��̏����֐�.
	*/
	void ConfigRender::pushButtonDown() {
		if (editingConfig && mainState < MAIN_STATE::KEYBOARD_SETTINGS) {
			editingConfig = false;
			playSoundEffectFromResource(7);
		}
	}

	/**
	*  �E�{�^��(����L�[)���������Ƃ��̏����֐�.
	*/
	void ConfigRender::pushButtonRight() {
		switch (state) {
		case STATE::MAIN:
			if (!editingConfig) {
				editingConfig = true;
				switch (mainState) {
				case MAIN_STATE::SCREEN_MODE:
					if (configData[0][0].value == "WINDOW")
						editCursorNumber = 1;
					else
						editCursorNumber = 2;
					for (int i = 0; i < 5; i++) {
						screenModeText[i].enableDraw();
					}
					break;
				case MAIN_STATE::V_SYNC:
					if (configData[1][0].value == "ON")
						editCursorNumber = 1;
					else
						editCursorNumber = 2;
					for (int i = 0; i < 5; i++) {
						vSyncText[i].enableDraw();
					}
					break;
				case MAIN_STATE::AUTO_PLAY:
					if (configData[2][0].value == "ON")
						editCursorNumber = 1;
					else
						editCursorNumber = 2;
					for (int i = 0; i < 5; i++) {
						autoPlayText[i].enableDraw();
					}
					break;
				case MAIN_STATE::CUSTOM_SKIN:
					if (configData[2][1].value == "ON")
						editCursorNumber = 1;
					else
						editCursorNumber = 2;
					for (int i = 0; i < 5; i++) {
						customSkinText[i].enableDraw();
					}
					break;
				case MAIN_STATE::TIMING:
					parameterBuffer = std::atoi(configData[2][2].value.c_str());
					changeTimingMilliSecond(parameterBuffer);
					for (int i = 0; i < 3; i++) {
						timingText[i].enableDraw();
					}
					break;
				case MAIN_STATE::KEYBOARD_SETTINGS:
					settingKeyboard = 0;
					for (int i = 0; i < 3; i++) {
						keyboardText[i].enableDraw();
					}
					break;
				case MAIN_STATE::JOYSTICK_SETTINGS:
					settingJoystick = 0;
					for (int i = 0; i < 3; i++) {
						joystickText[i].enableDraw();
					}
					break;
				}
			}
			else {
				editingConfig = false;
				switch (mainState) {
				case MAIN_STATE::SCREEN_MODE:
					configText[1].setColor(D3DCOLOR_ARGB(255, 255, 0, 0));
					if (editCursorNumber == 1)
						changeConfig("window", "mode", "WINDOW");
					else
						changeConfig("window", "mode", "FULL_SCREEN");
					break;
				case MAIN_STATE::V_SYNC:
					configText[3].setColor(D3DCOLOR_ARGB(255, 255, 0, 0));
					if (editCursorNumber == 1)
						changeConfig("directx", "v-sync", "ON");
					else
						changeConfig("directx", "v-sync", "OFF");
					break;
				case MAIN_STATE::AUTO_PLAY:
					configText[5].setColor(D3DCOLOR_ARGB(255, 255, 0, 0));
					if (editCursorNumber == 1)
						changeConfig("play", "autoplay", "ON");
					else
						changeConfig("play", "autoplay", "OFF");
					break;
				case MAIN_STATE::CUSTOM_SKIN:
					configText[6].setColor(D3DCOLOR_ARGB(255, 255, 0, 0));
					if (editCursorNumber == 1)
						changeConfig("play", "custom-skin", "ON");
					else
						changeConfig("play", "custom-skin", "OFF");
					break;
				case MAIN_STATE::TIMING:
					configText[7].setColor(D3DCOLOR_ARGB(255, 255, 0, 0));
					changeConfig("play", "notes-timing", std::to_string(parameterBuffer));
					break;
				}
			}
			playSoundEffectFromResource(5);
			break;
		case STATE::SAVE:
			playSoundEffectFromResource(6);
			saveConfig(false);
			break;
		case STATE::BACK:
			playSoundEffectFromResource(7);
			frameNumber.setFrameNumber(FrameNumber::FRAME_NUMBER::SONG_SELECT_INIT);
			break;
			break;
		}
	}

	/**
	*  RETURN�L�[���������Ƃ��̏����֐�.
	*/
	void ConfigRender::pushReturn() {
		pushButtonRight();
	}

	/**
	*  ESCAPE�L�[���������Ƃ��̏����֐�.
	*/
	void ConfigRender::pushEscape() {
		if (editingConfig) {
			playSoundEffectFromResource(7);
			editingConfig = false;
			settingKeyboard = 0;
			settingJoystick = 0;
		}
		else {
			playSoundEffectFromResource(7);
			state = STATE::BACK;
		}
	}

	/**
	*  �����o�ϐ��̕`��֐�.
	*/
	void ConfigRender::draw() {
		Sprite->Begin(NULL);

		Background.Draw();
		Title.Draw();
		Cursor.Draw();
		Buttons.Draw();
		for (int i = 0; i < (int)configText.size(); i++) {
			configText[i].Rend();
		}
		SaveComplate.Draw();
		Black.Draw();
		Frame1.Draw();
		EditCursor.Draw();

		for (int i = 0; i < 5; i++) {
			vSyncText[i].Rend();
			screenModeText[i].Rend();
			autoPlayText[i].Rend();
			customSkinText[i].Rend();
		}
		for (int i = 0; i < 3; i++) {
			timingText[i].Rend();
			keyboardText[i].Rend();
			joystickText[i].Rend();
		}

		Sprite->End();
	}
}