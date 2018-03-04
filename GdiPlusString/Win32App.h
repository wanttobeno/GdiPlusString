#pragma once
#include <Windows.h>

namespace Gdiplus
{
class Graphics;

}

class CWin32App
{
public:
	CWin32App(void);
	~CWin32App(void);
public:
	BOOL Create(HINSTANCE hinst);
	BOOL Run();
	BOOL InitializeObjects();
	void RenderScene();
	void Shutdown();
	HRESULT OnPaint(HWND hWnd,WPARAM wParam, LPARAM lParam);

	void DrawFenLan(HWND hWnd,Gdiplus::Graphics &graphics);

	void DrawMeasureString(Gdiplus::Graphics &graphics);

	void DrawFontInfo(Gdiplus::Graphics &graphics);

	LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	HWND		m_hWnd;
};
