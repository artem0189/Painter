#include <windows.h>
#include <vector>
#include "WindowPaint.h"

#define LAST_IND(vector) vector.size() - 1

BOOL fDraw;
RECT clientRect;
WindowPaint* window;
INT figureNumber = 1;
std::vector<POINT> points;
std::vector<WCHAR> letters;

VOID Draw(HDC hDc)
{
	switch (figureNumber) {
	case 1:
		MoveToEx(hDc, points[LAST_IND(points) - 1].x, points[LAST_IND(points) - 1].y, NULL);
		LineTo(hDc, points[LAST_IND(points)].x, points[LAST_IND(points)].y);
		break;
	case 2:
		Polyline(hDc, &points[0], points.size());
		break;
	case 3:
		Rectangle(hDc, points[LAST_IND(points) - 1].x, points[LAST_IND(points) - 1].y, points[LAST_IND(points)].x, points[LAST_IND(points)].y);
		break;
	case 4:
		Polygon(hDc, &points[0], points.size());
		break;
	case 5:
		Ellipse(hDc, points[LAST_IND(points) - 1].x, points[LAST_IND(points) - 1].y, points[LAST_IND(points)].x, points[LAST_IND(points)].y);
		break;
	case 6:
		TextOut(hDc, points[LAST_IND(points)].x, points[LAST_IND(points)].y, &letters[0], letters.size());
		break;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CREATE:
		fDraw = false;
		GetClientRect(hWnd, &clientRect);
		window = new WindowPaint(clientRect);
		break;
	case WM_COMMAND:
		figureNumber = LOWORD(wParam);
		points.clear();
		break;
	case WM_KEYDOWN:
		if (fDraw && figureNumber == 6) {
			window->UpdateWindow(hWnd, clientRect);
			letters.push_back(static_cast<WCHAR>(wParam));
			HDC hDc = GetDC(hWnd);
			Draw(hDc);
			ReleaseDC(hWnd, hDc);
		}
		break;
	case WM_LBUTTONDOWN:
		if (!fDraw) {
			if (points.size() == 0) {
				points.push_back({ LOWORD(lParam), HIWORD(lParam) });
			}
			window->SaveWindow(hWnd);
			points.push_back({ LOWORD(lParam), HIWORD(lParam) });
		}
		else {
			if (figureNumber == 2 || figureNumber == 4) {
				window->SaveWindow(hWnd);
				points.push_back({ LOWORD(lParam), HIWORD(lParam) });
				fDraw = !fDraw;
			}
			else {
				points.clear();
				letters.clear();
			}
		}
		fDraw = !fDraw;
		break;
	case WM_RBUTTONDOWN:
		if (fDraw) {
			window->UpdateWindow(hWnd, clientRect);
			points.clear();
			fDraw = !fDraw;
		}
		break;
	case WM_MOUSEMOVE:
		if (fDraw) {
			window->UpdateWindow(hWnd, clientRect);
			points[points.size() - 1] = { LOWORD(lParam), HIWORD(lParam) };
			HDC hDc = GetDC(hWnd);
			Draw(hDc);
			ReleaseDC(hWnd, hDc);
		}
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
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hPopupMenu, L"Фигуры");
	{
		AppendMenu(hPopupMenu, MF_STRING, 1, L"Прямая линия");
		AppendMenu(hPopupMenu, MF_STRING, 2, L"Ломаная линия");
		AppendMenu(hPopupMenu, MF_STRING, 3, L"Прямоугольник");
		AppendMenu(hPopupMenu, MF_STRING, 4, L"Многоугольник");
		AppendMenu(hPopupMenu, MF_STRING, 5, L"Эллипс");
		AppendMenu(hPopupMenu, MF_STRING, 6, L"Текст");
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