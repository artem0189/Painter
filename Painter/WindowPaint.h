#pragma once
#include <windows.h>
#include <atlimage.h>
#include <atltypes.h>

class WindowPaint {
public:
	WindowPaint();
	~WindowPaint();
	VOID SaveWindow(HWND hWnd, RECT rect);
	VOID UpdateWindow(HWND hWnd, RECT rect);
private:
	ATL::CImage* image = new CImage();
};