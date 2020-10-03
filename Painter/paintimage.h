#pragma once
#include <windows.h>
#include <atlimage.h>
#include <atltypes.h>

class Window {
public:
	void save_window(HWND hWnd, RECT rect);
	void update_window(HWND hWnd, RECT rect);
private:
	ATL::CImage* image;
};