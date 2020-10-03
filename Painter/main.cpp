#include <windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT clientRect;

	switch (uMsg) {
	case WM_CREATE:
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 1:
			MessageBox(hWnd, L"������ �����", L"gr", 0);
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_MOUSEMOVE:
		break;
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
	HMENU hMenu, hPopupMenu;
	MSG msg;
	WNDCLASSEX wc;

	memset(&wc, 0, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(wc);
	wc.lpszClassName = L"MainWindow";
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInst;

	RegisterClassEx(&wc);
	hMainWnd = CreateWindowEx(NULL, L"MainWindow", L"Painter", WS_OVERLAPPED | WS_SYSMENU, 100, 100, 500, 500, NULL, NULL, hInst, NULL);

	hMenu = CreateMenu();
	hPopupMenu = CreateMenu();
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hPopupMenu, L"������");
	{
		AppendMenu(hPopupMenu, MF_STRING, 1, L"������ �����");
		AppendMenu(hPopupMenu, MF_STRING, 2, L"������� �����");
		AppendMenu(hPopupMenu, MF_STRING, 3, L"�������������");
		AppendMenu(hPopupMenu, MF_STRING, 4, L"�������������");
		AppendMenu(hPopupMenu, MF_STRING, 5, L"������");
		AppendMenu(hPopupMenu, MF_STRING, 6, L"�����");
	}
	SetMenu(hMainWnd, hMenu);

	ShowWindow(hMainWnd, nCmdShow);
	UpdateWindow(hMainWnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}