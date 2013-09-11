#include <Windows.h>
#include <memory>
#include "BlankDemo.h"
#include "TriangleDemo.h"

using namespace std;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
						 WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
					LPWSTR cmdLine, int cmdShow)
{
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);

	LPCWSTR bb = L"说好的中文呢";
	WNDCLASSEXW wndClass = {0};
	wndClass.cbSize = sizeof(WNDCLASSEXW);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hInstance;
	//wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"DX11BookWindowClass";

	if (!RegisterClassExW(&wndClass))
		return -1;

	RECT rc = {0, 0, 640, 360};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindowW(wndClass.lpszClassName, bb, 
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left,
		rc.bottom - rc.top, NULL, NULL, hInstance, NULL);

	if (!hwnd)
		return -1;

	ShowWindow(hwnd, cmdShow);

	// Initialize our demo
	auto_ptr<Dx11DemoBase> demo(new TriangleDemo());
	BOOL result = demo->Initialize(hInstance, hwnd);

	if (!result)
		return -1;

	// Initialize
	MSG msg = {0};
	FLOAT hit = 0;
	while (msg.message != WM_QUIT)
	{
		hit++;
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//Update
			//Draw
			demo->Update(hit);
			demo->Render();
		}
	}

	

	demo->Shutdown();

	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
						 WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	HDC hDC;

	switch (msg)
	{
	case WM_PAINT:
		hDC = BeginPaint(hwnd, &paintStruct);
		EndPaint(hwnd, &paintStruct);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}