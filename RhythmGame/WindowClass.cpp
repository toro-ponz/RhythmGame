#define _CRT_SECURE_NO_WARNINGS

#include "WindowClass.h"
#include "GlobalVariable.h"

namespace Win {
	/**
	*  �R���X�g���N�^
	*  @param hInst<HINSTANCE> 
	*  @param nWinMode<int> 
	*/
	Window::Window(HINSTANCE hInst, int nWinMode) {
		loadWindowSettings();

		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);                                 //WNDCLASSEX�\���̂̃T�C�Y��ݒ�
		wc.style = NULL;                                                //�E�B���h�E�X�^�C���i�f�t�H���g�j
		wc.lpfnWndProc = WinProc;                                       //�E�B���h�E�֐�
		wc.cbClsExtra = 0;                                              //�ʏ�͎g��Ȃ��i�O�ɂ��Ă����j
		wc.cbWndExtra = 0;                                              //�ʏ�͎g��Ȃ��i�O�ɂ��Ă����j
		wc.hInstance = hInst;                                           //���̃C���X�^���X�ւ̃n���h��
		wc.hIcon = NULL;                                                //���[�W�A�C�R���i�Ȃ��j
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);                       //�J�[�\���X�^�C��
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);         //�E�B���h�E�̔w�i�i���j
		wc.lpszMenuName = NULL;                                         //���j���[�i�Ȃ��j
		wc.lpszClassName = windowName;                                  //�E�B���h�E�N���X��
		wc.hIconSm = NULL;                                              //�X���[���A�C�R���i�Ȃ��j

		if (!RegisterClassEx(&wc)) return;
		if (windowMode) {
			hWnd = CreateWindowEx(
				NULL,
				windowName,                                             //�E�B���h�E�N���X�̖��O
				windowTitle,                                            //�E�B���h�E�^�C�g��
				WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME,   //�E�B���h�E�X�^�C��
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,                                          //�E�B���h�E�̕�
				CW_USEDEFAULT,                                          //�E�B���h�E�̍���
				NULL,                                                   //�e�E�B���h�E�i�Ȃ��j
				NULL,                                                   //���j���[�i�Ȃ��j
				hInst,                                                  //���̃v���O�����̃C���X�^���X�̃n���h��
				NULL                                                    //�ǉ������i�Ȃ��j
			);
			RECT wRect, cRect;
			int ww, wh;
			int cw, ch;
			int Screen_PosX, Screen_PosY;
			GetWindowRect(hWnd, &wRect);
			ww = wRect.right - wRect.left;
			wh = wRect.bottom - wRect.top;
			GetClientRect(hWnd, &cRect);
			cw = cRect.right - cRect.left;
			ch = cRect.bottom - cRect.top;
			ww = ww - cw;
			wh = wh - ch;
			ww = SCREEN_WIDTH + ww;
			wh = SCREEN_HEIGHT + wh;
			Screen_PosX = (GetSystemMetrics(SM_CXSCREEN) - ww) / 2;
			Screen_PosY = (GetSystemMetrics(SM_CYSCREEN) - wh) / 2;
			hWnd = CreateWindowEx(
				NULL,
				windowName,                                             //�E�B���h�E�N���X�̖��O
				windowTitle,                                            //�E�B���h�E�^�C�g��
				WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME,   //�E�B���h�E�X�^�C��
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,                                          //�E�B���h�E�̕�
				CW_USEDEFAULT,                                          //�E�B���h�E�̍���
				NULL,                                                   //�e�E�B���h�E�i�Ȃ��j
				NULL,                                                   //���j���[�i�Ȃ��j
				hInst,                                                  //���̃v���O�����̃C���X�^���X�̃n���h��
				NULL                                                    //�ǉ������i�Ȃ��j
			);
			SetWindowPos(hWnd, HWND_TOP, Screen_PosX, Screen_PosY, ww, wh, NULL);
		}
		else {
			hWnd = CreateWindowEx(
				NULL,
				windowName,
				windowTitle,
				WS_POPUP,
				300,
				CW_USEDEFAULT,
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				NULL,
				NULL,
				hInst,
				NULL
			);
		}
		ShowWindow(hWnd, nWinMode); //�E�B���h�E��\��
		ValidateRect(hWnd, 0);		//WM_PAINT���Ă΂�Ȃ��悤�ɂ���
		UpdateWindow(hWnd);			//�E�B���h�E�̍X�V

		D3DDISPLAYMODE  DispMode;	//�f�B�X�v���C���[�h
		HRESULT         hr;
		Direct3D9 = Direct3DCreate9(D3D_SDK_VERSION);
		if (!Direct3D9) {           //�I�u�W�F�N�g�������s
			MessageBox(hWnd, _T("DirectXD3D9�I�u�W�F�N�g�������s"), _T("ERROR"), MB_OK);
		}
		//�f�B�X�v���C�f�[�^�i�[�\���̏�����
		ZeroMemory(&D3DPresentParameters, sizeof(D3DPRESENT_PARAMETERS));
		//���݂̃f�B�X�v���C���[�h�f�[�^�擾
		Direct3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &DispMode);
		//�o�b�N�o�b�t�@�t�H�[�}�b�g���f�B�X�v���C�Ɠ�����
		D3DPresentParameters.BackBufferFormat = DispMode.Format;
		//���h�b�g���ݒ�
		D3DPresentParameters.BackBufferWidth = SCREEN_WIDTH;
		//�c�h�b�g���ݒ�
		D3DPresentParameters.BackBufferHeight = SCREEN_HEIGHT;
		//�o�b�N�o�b�t�@�̐�
		D3DPresentParameters.BackBufferCount = 1;
		//�t���b�v�̕��@�i�ʏ�͂��̒萔�ł悢�j
		D3DPresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
		//�E�C���h�E���[�h�̐ݒ�
		D3DPresentParameters.Windowed = windowMode;
		//�X�e���V��ONOFF
		D3DPresentParameters.EnableAutoDepthStencil = FALSE;
		//�X�e���V���t�H�[�}�b�g
		D3DPresentParameters.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
		//��������OFF
		if (!vSync)
			D3DPresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		hr = Direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING, &D3DPresentParameters, &Direct3DDevice9);
		if (FAILED(hr)) {
			//�n�[�h�E�F�A�f�o�C�X�̐��������݂�
			hr = Direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING, &D3DPresentParameters, &Direct3DDevice9);
			if (FAILED(hr)) {
				//�\�t�g�E�F�A�f�o�C�X�̐��������݂�
				hr = Direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING, &D3DPresentParameters, &Direct3DDevice9);
				if (FAILED(hr)) {
					//--�ǂ̃f�o�C�X���擾�ł��Ȃ�����
					MessageBox(hWnd, _T("DirectX�f�o�C�X�������s"), _T("ERROR"), MB_OK);
				}
			}
		}

		D3DXCreateSprite(Direct3DDevice9, &Sprite);
		Direct3DDevice9->SetRenderState(D3DRS_LIGHTING, FALSE);
		Direct3DDevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		Direct3DDevice9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		Direct3DDevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		//SetTimer(hWnd, 0, 15, NULL);
		inputDevice = new InputDevice(hWnd);
	}

	/**
	*  �f�X�g���N�^
	*/
	Window::~Window() {
		if (Sprite != NULL) {
			Sprite->Release();
			Sprite = NULL;
		}
		if (Direct3DDevice9 != NULL) {
			Direct3DDevice9->Release();
			Direct3DDevice9 = NULL;
		}
		if (Direct3D9 != NULL) {
			Direct3D9->Release();
			Direct3D9 = NULL;
		}
		if (inputDevice != NULL) {
			delete inputDevice;
		}
	}

	/**
	*  �`�悷��֐�
	*/
	void Window::Rend() {
		rendering = true;
		if (isWindowActive) {
			Direct3DDevice9->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
			if (SUCCEEDED(Direct3DDevice9->BeginScene())) {
				Render.Rend();
				Direct3DDevice9->EndScene();
			}
			Direct3DDevice9->Present(NULL, NULL, NULL, NULL);
		}
		rendering = false;
	}

	bool Window::getRendering() {
		return rendering;
	}

	/**
	*  �E�B���h�E�Ɋւ���ݒ��ǂݍ��ފ֐�.
	*/
	void Window::loadWindowSettings() {
		TCHAR buf[256];
		GetPrivateProfileString(_T("window"), _T("mode"), _T("WINDOW"), buf, sizeof(buf), _T("Data/conf.ini"));
		if (!_tcscmp(buf, _T("FULL_SCREEN"))) windowMode = false;
		else windowMode = true;
		GetPrivateProfileString(_T("directx"), _T("v-sync"), _T("ON"), buf, sizeof(buf), _T("Data/conf.ini"));
		if (!_tcscmp(buf, _T("OFF"))) vSync = false;
		else vSync = true;
	}

	/**
	*  �E�B���h�E�v���V�[�W���֐�.
	*  @param hWnd<HWND> 
	*  @param message<UINT> 
	*  @param wParam<WPARAM> 
	*  @param lParam<LPARAM> 
	*  @return <LRESULT> 
	*/
	LRESULT CALLBACK Window::WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message) {
		case WM_ACTIVATE:
			if (wParam == WA_INACTIVE) {
				if (((wParam >> 16) & 0xFFFF) == 0) {
					isWindowActive = true;
				}
				else {
					isWindowActive = false;
				}
			}
			else {
				isWindowActive = true;
				if (inputDevice != NULL) {
					if (inputDevice->getEnabled()) {
						inputDevice->acquire();
					}
				}
				else {
					inputDevice = new Device::InputDevice(hWnd);
				}
			}
			break;
		case WM_MOUSEMOVE:
			break;
		case WM_KEYDOWN:
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}
}