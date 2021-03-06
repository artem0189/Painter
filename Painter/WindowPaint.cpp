#include "WindowPaint.h"

WindowPaint::WindowPaint(RECT rect)
{
	WindowPaint::image = new CImage();
	image->Create(rect.right - rect.left, rect.bottom - rect.top, 32);
}

WindowPaint::~WindowPaint()
{
	delete image;
}

VOID WindowPaint::SaveWindow(HWND hWnd)
{
	HDC imageDc = image->GetDC();
	PrintWindow(hWnd, imageDc, PW_CLIENTONLY);
	image->ReleaseDC();
}

VOID WindowPaint::UpdateWindow(HWND hWnd, RECT rect)
{
	HDC hDc = GetDC(hWnd);
	HDC imageDc = image->GetDC();
	BitBlt(hDc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, imageDc, rect.left, rect.top, SRCCOPY);
	image->ReleaseDC();
	ReleaseDC(hWnd, hDc);
}