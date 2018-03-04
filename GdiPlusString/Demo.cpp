#include <Windows.h>
#include <GdiPlus.h>
#include "Win32App.h"
using namespace Gdiplus;
#pragma comment(lib,"gdiplus.lib")

int WINAPI WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd)
{
	int nRet = 0;
	typedef unsigned long ULONG_PTR;
	ULONG_PTR m_gdiplusToken;
	Gdiplus::GdiplusStartupInput StartupInput;
	GdiplusStartup(&m_gdiplusToken, &StartupInput, NULL);

	CWin32App win32;
	if (win32.Create(hInstance))
		nRet = win32.Run();

	Gdiplus::GdiplusShutdown(m_gdiplusToken);
	return nRet;
}