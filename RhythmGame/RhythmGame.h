#pragma once
#include "WindowClass.h"
#include "InputDevice.h"

using namespace Win;
using namespace Device;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);

HWND hWnd;

LPDIRECTINPUT8 Device::Joystick::DirectInput8;
LPDIRECTINPUTDEVICE8 Device::Joystick::DirectInputDevice8;
Render * Win::Window::Render;
bool Win::Window::isWindowActive;

LPDIRECT3DDEVICE9 Direct3DDevice9;
LPDIRECT3D9 Direct3D9 = NULL;
LPD3DXSPRITE Sprite;
BYTE FrameNumber;
InputDevice *inputDevice;