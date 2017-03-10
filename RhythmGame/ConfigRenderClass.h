#pragma once

#include "SpriteDrawingClass.h"
#include "FontTextureCreateClass.h"

using namespace DxSprite;
using namespace Font;

namespace Frame {
	class ConfigRender {
	public:
		ConfigRender();
		~ConfigRender();
		void Rend();

	private:
		void editConfig();
		bool saveConfig(bool = true);
		bool loadConfig();
		bool changeConfig(string, string, string);
		bool initConfigData();

		void configTextCreate(int);
		void textCreate();
		void timingTextCreate();
		void changeTimingMilliSecond(int);
		void inputDeviceSettingsTextCreate();
		void keyboardSettingsKeyTextCreate(bool = false);
		void joystickSettingsKeyTextCreate(bool = false);
		void setKeyboard();
		void setJoystick();
		void saveKeyboard();
		void saveJoystick();

		void pushArrowUp();
		void pushArrowDown();
		void pushArrowLeft();
		void pushArrowRight();
		void pushButtonDown();
		void pushButtonRight();
		void pushReturn();
		void pushEscape();

		void draw();

		SpriteDrawing Background;
		SpriteDrawing Title;
		SpriteDrawing Buttons;
		SpriteDrawing Cursor;
		SpriteDrawing SaveComplate;
		SpriteDrawing Frame1;
		SpriteDrawing EditCursor;
		SpriteDrawing Black;

		vector<FontTextureCreate> configText;
		vector<FontTextureCreate> screenModeText;
		vector<FontTextureCreate> vSyncText;
		vector<FontTextureCreate> autoPlayText;
		vector<FontTextureCreate> customSkinText;
		vector<FontTextureCreate> timingText;
		vector<FontTextureCreate> keyboardText;
		vector<FontTextureCreate> joystickText;

		DWORD saveComplateTextDrawTime = 0;
		
		const string fontName = "ÉÅÉCÉäÉI";

		struct CONFIG_DATA {
			const string key;
			string value;
		};

		vector<vector<CONFIG_DATA>> configData;
		vector<string> sectionName;

		int editCursorNumber;
		int selectedSectionNumber;
		int selectedKeyNumber;
		int cursorNumber;
		int parameterBuffer;
		int settingKeyboard;
		int settingJoystick;

		int inputDeviceParameterBuffer[12];

		bool editingConfig;
		bool changedKeyboardSettings;
		bool changedJoystickSettings;

		enum class STATE {
			MAIN,
			SAVE,
			BACK
		} state;

		enum class MAIN_STATE {
			SCREEN_MODE,
			V_SYNC,
			AUTO_PLAY,
			CUSTOM_SKIN,
			TIMING,
			KEYBOARD_SETTINGS,
			JOYSTICK_SETTINGS
		} mainState;
	};
}