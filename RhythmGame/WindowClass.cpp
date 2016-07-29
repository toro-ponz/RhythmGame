#define _CRT_SECURE_NO_WARNINGS

#include "WindowClass.h"
#include "InputDeviceClass.h"

extern LPDIRECT3DDEVICE9 gl_lpD3ddev;
extern LPDIRECT3D9 gl_lpD3d;
extern LPD3DXSPRITE g_pSprite;
extern InputDevice * inputDevice;
extern HWND hWnd;

namespace Win {
	//�R���X�g���N�^
	Window::Window(HINSTANCE hInst, int nWinMode) {
		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);                            // WNDCLASSEX�\���̂̃T�C�Y��ݒ�
		wc.style = NULL;                                           // �E�B���h�E�X�^�C���i�f�t�H���g�j
		wc.lpfnWndProc = WinProc;                                  // �E�B���h�E�֐�
		wc.cbClsExtra = 0;                                         // �ʏ�͎g��Ȃ��i�O�ɂ��Ă����j
		wc.cbWndExtra = 0;                                         // �ʏ�͎g��Ȃ��i�O�ɂ��Ă����j
		wc.hInstance = hInst;                                      // ���̃C���X�^���X�ւ̃n���h��
		wc.hIcon = NULL;                                           // ���[�W�A�C�R���i�Ȃ��j
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);                  // �J�[�\���X�^�C��
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);    // �E�B���h�E�̔w�i�i���j
		wc.lpszMenuName = NULL;                                    // ���j���[�i�Ȃ��j
		wc.lpszClassName = szWinName;                              // �E�B���h�E�N���X��
		wc.hIconSm = NULL;                                         // �X���[���A�C�R���i�Ȃ��j

		if (!RegisterClassEx(&wc)) return;
		LPCWSTR lpWinName = (LPCWSTR)szWinName;
		LPCWSTR lpWinTitle = (LPCWSTR)szWinTitle;
		if (WINMODE) {
			hWnd = CreateWindowEx(
				NULL,
				szWinName,                                         // �E�B���h�E�N���X�̖��O
				szWinTitle,                                        // �E�B���h�E�^�C�g��
				WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE,    // �E�B���h�E�X�^�C��
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,                                     // �E�B���h�E�̕�
				CW_USEDEFAULT,                                     // �E�B���h�E�̍���
				NULL,                                              // �e�E�B���h�E�i�Ȃ��j
				NULL,                                              // ���j���[�i�Ȃ��j
				hInst,                                             // ���̃v���O�����̃C���X�^���X�̃n���h��
				NULL                                               // �ǉ������i�Ȃ��j
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
			SetWindowPos(hWnd, HWND_TOP, Screen_PosX, Screen_PosY, ww, wh, NULL);
		}
		else {
			hWnd = CreateWindowEx(
				NULL,
				szWinName,
				szWinTitle,
				WS_VISIBLE | WS_POPUP,
				0,
				0,
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				NULL,
				NULL,
				hInst,
				NULL
			);
		}
		ShowWindow(hWnd, nWinMode); // �E�B���h�E��\��
		ValidateRect(hWnd, 0);		// WM_PAINT���Ă΂�Ȃ��悤�ɂ���
		UpdateWindow(hWnd);			// �E�B���h�E�̍X�V

		D3DDISPLAYMODE  DispMode;	// �f�B�X�v���C���[�h
		HRESULT         hr;
		gl_lpD3d = Direct3DCreate9(D3D_SDK_VERSION);
		if (!gl_lpD3d) {      // �I�u�W�F�N�g�������s
			MessageBox(hWnd, _T("DirectXD3D9�I�u�W�F�N�g�������s"), _T("ERROR"), MB_OK);
		}
		// �f�B�X�v���C�f�[�^�i�[�\���̏�����
		ZeroMemory(&gl_d3dpp, sizeof(D3DPRESENT_PARAMETERS));
		// ���݂̃f�B�X�v���C���[�h�f�[�^�擾
		gl_lpD3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &DispMode);
		// �o�b�N�o�b�t�@�t�H�[�}�b�g���f�B�X�v���C�Ɠ�����
		gl_d3dpp.BackBufferFormat = DispMode.Format;
		// ���h�b�g���ݒ�
		gl_d3dpp.BackBufferWidth = SCREEN_WIDTH;
		// �c�h�b�g���ݒ�
		gl_d3dpp.BackBufferHeight = SCREEN_HEIGHT;
		// �o�b�N�o�b�t�@�̐�
		gl_d3dpp.BackBufferCount = 1;
		// �t���b�v�̕��@�i�ʏ�͂��̒萔�ł悢�j
		gl_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		// �E�C���h�E���[�h�̐ݒ�
		gl_d3dpp.Windowed = WINMODE;
		//�X�e���V��ONOFF
		gl_d3dpp.EnableAutoDepthStencil = FALSE;
		// �X�e���V���t�H�[�}�b�g
		gl_d3dpp.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
		//��������OFF
		//gl_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		hr = gl_lpD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING, &gl_d3dpp, &gl_lpD3ddev);
		if (FAILED(hr)) {
			//�n�[�h�E�F�A�f�o�C�X�̐��������݂�
			hr = gl_lpD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING, &gl_d3dpp, &gl_lpD3ddev);
			if (FAILED(hr)) {
				//�\�t�g�E�F�A�f�o�C�X�̐��������݂�
				hr = gl_lpD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING, &gl_d3dpp, &gl_lpD3ddev);
				if (FAILED(hr)) {
					//--�ǂ̃f�o�C�X���擾�ł��Ȃ�����
					MessageBox(hWnd, _T("DirectX�f�o�C�X�������s"), _T("ERROR"), MB_OK);
				}
			}
		}

		D3DXCreateSprite(gl_lpD3ddev, &g_pSprite);
		gl_lpD3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
		gl_lpD3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		gl_lpD3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		gl_lpD3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		SetTimer(hWnd, 0, 15, NULL);
		inputDevice = new InputDevice(hWnd);
		Render = new Frame::Render();
	}

	//�f�X�g���N�^
	Window::~Window() {
		if (g_pSprite != NULL) {
			g_pSprite->Release();
			g_pSprite = NULL;
		}
		if (gl_lpD3ddev != NULL) {
			gl_lpD3ddev->Release();
			gl_lpD3ddev = NULL;
		}
		if (gl_lpD3d != NULL) {
			gl_lpD3d->Release();
			gl_lpD3d = NULL;
		}
		if (inputDevice != NULL) {
			delete inputDevice;
		}
		if (Render != NULL) {
			delete Render;
		}
	}

	//�E�B���h�E�v���V�[�W��
	LRESULT CALLBACK Window::WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message) {
		case WM_ACTIVATE:
			if (wParam == WA_INACTIVE) {
				if (((wParam >> 16) & 0xFFFF) == 0) {
					g_appActive = true;
				}
				else {
					g_appActive = false;
				}
			}
			else {
				g_appActive = true;
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
			switch (wParam) {
			case VK_ESCAPE:
				PostMessage(hWnd, WM_CLOSE, 0, 0);
				break;
			}
			break;
		case WM_TIMER:
			if (g_appActive) {
				gl_lpD3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
				if (SUCCEEDED(gl_lpD3ddev->BeginScene())) {
					Render->Rend();
					gl_lpD3ddev->EndScene();
				}
				gl_lpD3ddev->Present(NULL, NULL, NULL, NULL);
			}
			break;
		case WM_DESTROY:        // ����{�^�����N���b�N������
			PostQuitMessage(0);
			break;
		default: // ��L�ȊO�̃��b�Z�[�W��Windows�֏�����C����
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}
}