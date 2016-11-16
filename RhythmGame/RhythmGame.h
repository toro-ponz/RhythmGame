#pragma once

#include "WindowClass.h"
#include "InputDevice.h"

using namespace Win;
using namespace Device;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);

LPDIRECTINPUT8 Device::Joystick::DirectInput8;
LPDIRECTINPUTDEVICE8 Device::Joystick::DirectInputDevice8;
bool Win::Window::isWindowActive;