
#include <windows.h>
#include "DxInitializer.cpp"
#include "Timer.cpp"

class WindowCreator{
private:
	HINSTANCE hInstance;
	HINSTANCE hPrevInstance;
	LPSTR lpCmdLine;
	int nShowCmd;
	LPCTSTR WndClassName;
	DxInitializer* hardDevice;
	DxInitializer* softDevice;
	Timer *softTimer;
	Timer *hardTimer;
	int framesCount;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
		switch (msg)
		{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		} break;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	void WindowCreator::registerWindowClass(){
		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = this->WndProc;
		wc.cbClsExtra = NULL;
		wc.cbWndExtra = NULL;
		wc.hInstance = this->hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = this->WndClassName;
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		if (!RegisterClassEx(&wc))
		{
			MessageBox(NULL, L"Error registering class",
				L"Error", MB_OK | MB_ICONERROR);
			return;
		}
	}

public:
	bool ifAnyWindowClosed;
	WindowCreator::WindowCreator(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
		WndClassName = L"WindowClass";
		this->hInstance = hInstance;
		this->hPrevInstance = hPrevInstance;
		this->lpCmdLine = lpCmdLine;
		this->nShowCmd = nShowCmd;
		this->registerWindowClass();
		this->hardDevice = new DxInitializer();
		this->softDevice = new DxInitializer();
		this->framesCount = 100;
		this->ifAnyWindowClosed = false;
	};
	WindowCreator::WindowCreator(){

	};

	void WindowCreator::initializeHardwareWindow(){
		HWND hwnd = NULL;
		hwnd = CreateWindowEx(NULL, WndClassName, L"Hardware Rendering", (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU), CW_USEDEFAULT, CW_USEDEFAULT, 600, 600, NULL,
			NULL, hInstance,
			NULL
			);

		if (!hwnd)
		{
			MessageBox(NULL, L"Error creating window",
				L"Error", MB_OK | MB_ICONERROR);
			return;
		}

		this->hardTimer = new Timer(hwnd, this->framesCount);
		ShowWindow(hwnd, true);
		hardDevice->InitializeDirect3d11App(hwnd, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE);
		UpdateWindow(hwnd);


	}
	void WindowCreator::initializeSoftwareWindow(){
		HWND hwnd = NULL;
		hwnd = CreateWindowEx(NULL, WndClassName, L"Software Rendering", (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU), CW_USEDEFAULT, CW_USEDEFAULT, 600, 600, NULL,
			NULL, hInstance,
			NULL
			);

		if (!hwnd)
		{
			MessageBox(NULL, L"Error creating window",
				L"Error", MB_OK | MB_ICONERROR);
			return;
		}
		this->softTimer = new Timer(hwnd, this->framesCount);
		ShowWindow(hwnd, true);
		softDevice->InitializeDirect3d11App(hwnd, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_WARP);
		UpdateWindow(hwnd);

	}
	void WindowCreator::chceckEventMessages(int whichRefresh){
		MSG msg;
		while (true){
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				if (msg.message == WM_QUIT){
					hardDevice->ReleaseObjects();
					softDevice->ReleaseObjects();
					this->ifAnyWindowClosed = true;
					return;
				}
			}
			else{
				if (whichRefresh == 1){
					hardDevice->UpdateScene();
					hardTimer->countTime();
					hardDevice->DrawScene();
					hardTimer->refreshWindowIfShouldBe(L"Hardware Window: ");
				}
				else{
					softTimer->countTime();
					softDevice->UpdateScene();
					softDevice->DrawScene();
					softTimer->refreshWindowIfShouldBe(L"Software Window: ");
				}
			}
		}
	}
};
