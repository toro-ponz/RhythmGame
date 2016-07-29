#define _CRT_SECURE_NO_WARNINGS

#include "WindowClass.h"
#include "InputDeviceClass.h"

extern LPDIRECT3DDEVICE9 gl_lpD3ddev;
extern LPDIRECT3D9 gl_lpD3d;
extern LPD3DXSPRITE g_pSprite;
extern InputDevice * inputDevice;
extern HWND hWnd;

namespace Win {
	//コンストラクタ
	Window::Window(HINSTANCE hInst, int nWinMode) {
		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);                            // WNDCLASSEX構造体のサイズを設定
		wc.style = NULL;                                           // ウィンドウスタイル（デフォルト）
		wc.lpfnWndProc = WinProc;                                  // ウィンドウ関数
		wc.cbClsExtra = 0;                                         // 通常は使わない（０にしておく）
		wc.cbWndExtra = 0;                                         // 通常は使わない（０にしておく）
		wc.hInstance = hInst;                                      // このインスタンスへのハンドル
		wc.hIcon = NULL;                                           // ラージアイコン（なし）
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);                  // カーソルスタイル
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);    // ウィンドウの背景（黒）
		wc.lpszMenuName = NULL;                                    // メニュー（なし）
		wc.lpszClassName = szWinName;                              // ウィンドウクラス名
		wc.hIconSm = NULL;                                         // スモールアイコン（なし）

		if (!RegisterClassEx(&wc)) return;
		LPCWSTR lpWinName = (LPCWSTR)szWinName;
		LPCWSTR lpWinTitle = (LPCWSTR)szWinTitle;
		if (WINMODE) {
			hWnd = CreateWindowEx(
				NULL,
				szWinName,                                         // ウィンドウクラスの名前
				szWinTitle,                                        // ウィンドウタイトル
				WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE,    // ウィンドウスタイル
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,                                     // ウィンドウの幅
				CW_USEDEFAULT,                                     // ウィンドウの高さ
				NULL,                                              // 親ウィンドウ（なし）
				NULL,                                              // メニュー（なし）
				hInst,                                             // このプログラムのインスタンスのハンドル
				NULL                                               // 追加引数（なし）
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
		ShowWindow(hWnd, nWinMode); // ウィンドウを表示
		ValidateRect(hWnd, 0);		// WM_PAINTが呼ばれないようにする
		UpdateWindow(hWnd);			// ウィンドウの更新

		D3DDISPLAYMODE  DispMode;	// ディスプレイモード
		HRESULT         hr;
		gl_lpD3d = Direct3DCreate9(D3D_SDK_VERSION);
		if (!gl_lpD3d) {      // オブジェクト生成失敗
			MessageBox(hWnd, _T("DirectXD3D9オブジェクト生成失敗"), _T("ERROR"), MB_OK);
		}
		// ディスプレイデータ格納構造体初期化
		ZeroMemory(&gl_d3dpp, sizeof(D3DPRESENT_PARAMETERS));
		// 現在のディスプレイモードデータ取得
		gl_lpD3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &DispMode);
		// バックバッファフォーマットをディスプレイと等価に
		gl_d3dpp.BackBufferFormat = DispMode.Format;
		// 横ドット幅設定
		gl_d3dpp.BackBufferWidth = SCREEN_WIDTH;
		// 縦ドット幅設定
		gl_d3dpp.BackBufferHeight = SCREEN_HEIGHT;
		// バックバッファの数
		gl_d3dpp.BackBufferCount = 1;
		// フリップの方法（通常はこの定数でよい）
		gl_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		// ウインドウモードの設定
		gl_d3dpp.Windowed = WINMODE;
		//ステンシルONOFF
		gl_d3dpp.EnableAutoDepthStencil = FALSE;
		// ステンシルフォーマット
		gl_d3dpp.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
		//垂直同期OFF
		//gl_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		hr = gl_lpD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING, &gl_d3dpp, &gl_lpD3ddev);
		if (FAILED(hr)) {
			//ハードウェアデバイスの生成を試みる
			hr = gl_lpD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING, &gl_d3dpp, &gl_lpD3ddev);
			if (FAILED(hr)) {
				//ソフトウェアデバイスの生成を試みる
				hr = gl_lpD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING, &gl_d3dpp, &gl_lpD3ddev);
				if (FAILED(hr)) {
					//--どのデバイスも取得できなかった
					MessageBox(hWnd, _T("DirectXデバイス生成失敗"), _T("ERROR"), MB_OK);
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

	//デストラクタ
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

	//ウィンドウプロシージャ
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
		case WM_DESTROY:        // 閉じるボタンをクリックした時
			PostQuitMessage(0);
			break;
		default: // 上記以外のメッセージはWindowsへ処理を任せる
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}
}