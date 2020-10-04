#pragma once
#include <windows.h>
#include <atlimage.h>
#include <atltypes.h>

class WindowPaint {
public:
	WindowPaint(RECT rect);
	~WindowPaint();
	VOID SaveWindow(HWND hWnd);
	VOID UpdateWindow(HWND hWnd, RECT rect);
private:
	ATL::CImage* image = new CImage();
};