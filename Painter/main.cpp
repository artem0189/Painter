#include <windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(NULL);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return NULL;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hMainWnd;
	MSG msg;
	WNDCLASSEX wc;

	memset(&wc, 0, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(wc);
	wc.lpszClassName = L"MainWindow";
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInst;

	RegisterClassEx(&wc);
	hMainWnd = CreateWindowEx(NULL, L"MainWindow", L"Painter", WS_OVERLAPPED | WS_SYSMENU, 100, 100, 500, 500, NULL, NULL, hInst, NULL);
	ShowWindow(hMainWnd, nCmdShow);
	UpdateWindow(hMainWnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}