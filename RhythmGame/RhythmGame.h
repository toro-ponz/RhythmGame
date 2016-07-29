#pragma once
#include "WindowClass.h"
#include "InputDeviceClass.h"

using namespace Win;
using namespace Device;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);

HWND hWnd;

LPDIRECTINPUT8 Device::Joystick::lpDI;
LPDIRECTINPUTDEVICE8 Device::Joystick::lpDIDevice;
Render * Win::Window::Render;
bool Win::Window::g_appActive;

LPDIRECT3DDEVICE9 gl_lpD3ddev;
LPDIRECT3D9 gl_lpD3d = NULL;
LPD3DXSPRITE g_pSprite;
BYTE g_FrameNo;
InputDevice * inputDevice;