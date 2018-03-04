#include "Win32App.h"
#include <tchar.h>
#include "MemDC.h"
#include "WtlString.h"
#include <GdiPlus.h>
using namespace Gdiplus;
using namespace WTL;


#define WINDOW_CLASS    _T("Win32")
#define WINDOW_NAME     _T("GdiPlusString")
#define WINDOW_WIDTH    640
#define WINDOW_HEIGHT   480


CWin32App	*g_pApp=NULL;

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return g_pApp->WinProc(hWnd,msg,wParam,lParam);
}

CWin32App::CWin32App(void)
{
	m_hWnd = NULL;
	g_pApp= this;
}

CWin32App::~CWin32App(void)
{
}

BOOL CWin32App::Run()
{
	// Show the window
	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(m_hWnd);

	// Enter the message loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Sleep(5);
			RenderScene();
		}
	}
	Shutdown();
	return 0;
}

void CWin32App::Shutdown()
{

}

void CWin32App::RenderScene()
{
	
}

BOOL CWin32App::Create(HINSTANCE hinst)
{
	// Register the window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		WINDOW_CLASS, NULL };
	RegisterClassEx(&wc);

	// Create the application's window
	m_hWnd = CreateWindow(WINDOW_CLASS, WINDOW_NAME, WS_OVERLAPPEDWINDOW,
		100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, GetDesktopWindow(),
		NULL, wc.hInstance, NULL);

	BOOL bRet = IsWindow(m_hWnd);
	return bRet;
}

// 读取文件内容
CString g_FileStrBuf;

HRESULT CWin32App::OnPaint(HWND hWnd,WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd,&ps);
	RECT rtClient={0};
	GetClientRect(hWnd,&rtClient);
	{
		CMemDC  memDc(hdc,rtClient);

		Graphics graphics(memDc);
		graphics.Clear(Color::Gray);

		if (g_FileStrBuf.IsEmpty())
		{
			DrawFontInfo(graphics);

			// 测量文本
			graphics.TranslateTransform(100,150);
			DrawMeasureString(graphics);
			graphics.ResetTransform();
		}
		else
		{
			DrawFenLan(hWnd, graphics);
		}
	}
	EndPaint(hWnd,&ps);
	return 0;
}


void CWin32App::DrawFenLan(HWND hWnd, Graphics &graphics)
{
	// 选定要分栏显示的文件
	// 分栏显示
	// 定义栏宽及栏与栏之间的间隔
	int  COLUMWIDTH = 100;
	int  SPACE = 10;	

	SolidBrush brush(Color(255,0,0,0));
	Font myFont(L"宋体 ",10);

	// 得到当前窗口的区域
	RECT rect;
	GetClientRect(hWnd,&rect);
	RectF boundRect;
	INT cocepointsFitted = 0;
	INT linesFilled = 0;

	//设置输出格式 
	StringFormat format;

	// 禁用自动裁剪
	format.SetFormatFlags(StringFormatFlagsNoClip);

	// 输出文本以单词结束
	format.SetTrimming(StringTrimmingWord);

	// 从左至右按列（栏）输出文本
	CString outStr = g_FileStrBuf;
	for (int x=0; outStr.GetLength()>0 && x<rect.right;x+=(COLUMWIDTH+SPACE))
	{
		// 规定文本输出区域（一栏）
		RectF rectf(x,0,COLUMWIDTH,rect.bottom-myFont.GetHeight(0.0f));

		// 计算一栏中能够显示多少字符
		graphics.MeasureString(outStr,-1,&myFont,rectf,&format,&boundRect,&cocepointsFitted,&linesFilled);

		// 显示一栏文本
		graphics.DrawString(outStr,-1,&myFont,rectf,&format,&brush);
		// 去掉已经输出的字符，以便下一栏能够正常显示
		outStr = outStr.Mid(cocepointsFitted);
	}
}


void CWin32App::DrawMeasureString(Graphics &graphics)
{
	wchar_t string[256]=L"123456789ABCDEFGHIJKLM";
	FontFamily fontFamily(L"Arial");
	// 创建两个大小不同的Arial字体
	Font font(&fontFamily,30,FontStyleRegular,UnitPixel);
	Font font2(&fontFamily,18,FontStyleRegular,UnitPixel);
	// 设置文本输出矩形（错误的设置）
	// 高度为0的时候，才会返回自适应的矩形高度
	RectF layoutRect(10,10,130,90);
	SolidBrush brush(Color(255,0,0,0));
	RectF boundRect;
	INT codepointsFitted=0;
	INT linesFilled=0;

	int strlen = wcslen(string);

	//  测量文本 
	graphics.MeasureString(string,strlen,&font,layoutRect,NULL,
		&boundRect,&codepointsFitted,&linesFilled);

	CString tmp;
	CString s(string);
	tmp.Format(L"预输出的字符串为\n\"%s\"\n共%d个字符，\n其中，在规定的输入矩形中，\n只能输出%d行，共%d个字符",
				s,s.GetLength(),linesFilled,codepointsFitted);

	CString msg(tmp);
	// 绘制文本输出矩形和限制矩形
	graphics.DrawRectangle(&Pen(Color(255,0,255,255),2),layoutRect);
	graphics.DrawRectangle(&Pen(Color(255,0,0,0)),boundRect);

	// 输出字符串的测量信息
	graphics.DrawString(string,-1,&font,boundRect,NULL,&brush);
	graphics.DrawString(msg,-1,&font2,PointF(10,180),NULL,&brush);
}

void CWin32App::DrawFontInfo(Graphics &graphics)
{
	PointF pointF(0.0f,0.0f);
	SolidBrush solidBrush(Color(255,0,0,0));
	wchar_t infoSting[100]=L"";
	REAL FontHeightPixel;
	UINT ascent;
	REAL ascentPixel;
	UINT descent;
	REAL descentPixel;
	UINT lineSpacing;
	REAL lineSpacingPixel;

	FontFamily fontFamily(L"Arial");
	// 创建一个大小为16像素的Arial字体
	Font font(&fontFamily,16,FontStyleRegular,UnitPixel);

	// 显示字体大小
	swprintf(infoSting,L"使用font.GetSize()函数的返回值为： %.2f.",font.GetSize());
	graphics.DrawString(infoSting,wcslen(infoSting),&font,pointF,&solidBrush);
	// 下移一行
	pointF.Y += font.GetHeight(0.0f);
	// 显示字体高度：像素
	swprintf(infoSting,L"使用font.GetHeight()函数的返回值为： %.2f像素",font.GetHeight(0.0));
	graphics.DrawString(infoSting,wcslen(infoSting),&font,pointF,&solidBrush);
	// 下移一行
	pointF.Y += font.GetHeight(0.0f);

	// 显示字体系列的高度（设计单位）
	swprintf(infoSting,L"使用fontFamily.GetEmHeight()函数的返回值为： %d个设计单位。",fontFamily.GetEmHeight(FontStyleRegular));
	graphics.DrawString(infoSting,-1,&font,pointF,&solidBrush);


	// 下移两行（GetHeight返回的是字体的高度，单位为像素）
	pointF.Y +=2.0f*font.GetHeight(0.0f);

	// 获取字体的上部距离
	ascent = fontFamily.GetCellAscent(FontStyleRegular);

	//将上部距离的设计单位转换为像素单位
	ascentPixel =font.GetSize()*ascent/fontFamily.GetEmHeight(FontStyleRegular);
	swprintf(infoSting,L"上部距离为%d个设计单位， %.2f像素",ascent,ascentPixel);
	graphics.DrawString(infoSting,-1,&font,pointF,&solidBrush);

	// 垂直坐标下移一行
	pointF.Y += font.GetHeight(0.0f);

	// 获取字体的下部距离，设计单位
	descent = fontFamily.GetCellDescent(FontStyleRegular);

	// 将下部距离的设计单位转换为像素单位
	descentPixel  = font.GetSize()*descent/fontFamily.GetEmHeight(FontStyleRegular);
	swprintf(infoSting,L"下部距离为 %d个设计单位，%.2f 像素。",descent,descentPixel);
	graphics.DrawString(infoSting,-1,&font,pointF,&solidBrush);

	// 垂直坐标下移一行
	pointF.Y += font.GetHeight(0.0f);
	// 获取行距（设计单位）
	lineSpacing = fontFamily.GetLineSpacing(FontStyleRegular);

	// 将行设计单位转换成像素单位
	lineSpacingPixel = font.GetSize() * lineSpacing/fontFamily.GetEmHeight(FontStyleRegular);
	swprintf(infoSting,L"行距为 %d个设计单位，%.2f 像素。",lineSpacing,lineSpacingPixel);
	graphics.DrawString(infoSting,-1,&font,pointF,&solidBrush);
}

LRESULT WINAPI CWin32App::WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	case WM_RBUTTONUP:
		{
			OPENFILENAME ofn;       // common dialog box structure
			TCHAR szFile[260];       // buffer for file name
			HANDLE hf;              // file handle

			// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szFile;
			//
			// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
			// use the contents of szFile to initialize itself.
			//
			ofn.lpstrFile[0] = _T('\0');
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = _T("All\0*.*\0Text\0*.TXT\0");
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			// Display the Open dialog box. 
			if (GetOpenFileName(&ofn)==TRUE)
			{
				CString strOpenFileName;
				strOpenFileName = ofn.lpstrFile;
				// 打开文件操作是否真确

				hf = CreateFile(ofn.lpstrFile, GENERIC_READ,
					0, (LPSECURITY_ATTRIBUTES) NULL,
					OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
					(HANDLE) NULL);
				if (hf!=INVALID_HANDLE_VALUE)
				{
					DWORD dwSize =0;
					dwSize = GetFileSize(hf,NULL);

					DWORD dwBytesRead = 0;
					char *pReadBuffer = new char[dwSize];
					ReadFile(hf,pReadBuffer,dwSize,&dwBytesRead,NULL);
					g_FileStrBuf =pReadBuffer;
					CloseHandle(hf);
				}
			}
			::InvalidateRect(hWnd,NULL,false);
		}
		break;
	case WM_PAINT:
		OnPaint(hWnd,wParam,lParam);
		break;
	case WM_KEYUP:
		if(wParam == VK_ESCAPE) PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}