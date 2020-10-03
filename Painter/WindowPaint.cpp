#include "WindowPaint.h"

WindowPaint::WindowPaint()
{
	WindowPaint::image = new CImage();
}

WindowPaint::~WindowPaint()
{
	delete image;
}

VOID WindowPaint::SaveWindow(HWND hWnd, RECT rect)
{
	image->Create(rect.right - rect.left, rect.bottom - rect.top);
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