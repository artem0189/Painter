#include <windows.h>
#include <vector>
#include "WindowPaint.h"

#define LAST_IND(vector) vector.size() - 1

BOOL fDraw;
RECT clientRect;
WindowPaint* window;
INT figureNumber = 1;
COLORREF color = (COLORREF)RGB(255, 255, 255);
INT thickness = 1;
std::vector<POINT> points;
std::vector<WCHAR> letters;

VOID Draw(HDC hDc)
{
	SelectObject(hDc, CreateSolidBrush(color));
	SelectObject(hDc, CreatePen(PS_SOLID, thickness, (COLORREF)RGB(0, 0, 0)));

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
	PAINTSTRUCT ps;
	WORD param;

	switch (uMsg) {
	case WM_CREATE:
		fDraw = false;
		GetClientRect(hWnd, &clientRect);
		window = new WindowPaint(clientRect);
		break;
	case WM_COMMAND:
		param = LOWORD(wParam);
		if (param >= 1 && param <= 6) {
			figureNumber = LOWORD(wParam);
			points.clear();
		}

		if (param >= 100) {
			thickness = param / 100;
		}

		switch (param) {
		case 10:
			color = (COLORREF)RGB(255, 255, 255);
			break;
		case 11:
			color = (COLORREF)RGB(255, 0, 0);
			break;
		case 12:
			color = (COLORREF)RGB(0, 255, 0);
			break;
		case 13:
			color = (COLORREF)RGB(0, 0, 255);
			break;
		case 14:
			color = (COLORREF)RGB(0, 0, 0);
			break;
		}
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
			SendMessage(hWnd, WM_PAINT, 0, 0);
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
	HMENU hMenu, hPopupFigureMenu, hPopupColorMenu, hPopupThicknessMenu;
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
	hPopupFigureMenu = CreateMenu();
	hPopupColorMenu = CreateMenu();
	hPopupThicknessMenu = CreateMenu();
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hPopupFigureMenu, L"Фигуры");
	{
		AppendMenu(hPopupFigureMenu, MF_STRING, 1, L"Прямая линия");
		AppendMenu(hPopupFigureMenu, MF_STRING, 2, L"Ломаная линия");
		AppendMenu(hPopupFigureMenu, MF_STRING, 3, L"Прямоугольник");
		AppendMenu(hPopupFigureMenu, MF_STRING, 4, L"Многоугольник");
		AppendMenu(hPopupFigureMenu, MF_STRING, 5, L"Эллипс");
		AppendMenu(hPopupFigureMenu, MF_STRING, 6, L"Текст");
	}
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hPopupColorMenu, L"Цвет");
	{
		AppendMenu(hPopupColorMenu, MF_STRING, 10, L"Белый");
		AppendMenu(hPopupColorMenu, MF_STRING, 11, L"Красный");
		AppendMenu(hPopupColorMenu, MF_STRING, 12, L"Зелёный");
		AppendMenu(hPopupColorMenu, MF_STRING, 13, L"Синий");
		AppendMenu(hPopupColorMenu, MF_STRING, 14, L"Чёртный");
	}
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hPopupThicknessMenu, L"Толщина линии");
	{
		AppendMenu(hPopupThicknessMenu, MF_STRING, 100, L"1");
		AppendMenu(hPopupThicknessMenu, MF_STRING, 200, L"2");
		AppendMenu(hPopupThicknessMenu, MF_STRING, 400, L"4");
		AppendMenu(hPopupThicknessMenu, MF_STRING, 800, L"8");
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