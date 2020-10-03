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

}

VOID WindowPaint::UpdateWindow(HWND hWnd, RECT rect)
{

}