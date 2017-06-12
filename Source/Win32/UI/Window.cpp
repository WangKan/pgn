#define PGN_DLL_EXPORT
#include <PGN/Platform/UI/Window.h>
#undef PGN_DLL_EXPORT

#include <Windows.h>
#include <cassert>
#include <gl/gl.h>
#include <gl/wglext.h>
#include <list>
#include <map>
#include <PGN/Common/debug_new.h>
#include "Gesture/DraggingGestureRecognizer.h"
#include "WGL.h"

class GestureRecognizer;
extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;

class Window : public pgn::Window
{
public:
	HWND hWnd;
	HDC hDC;
	static const int maxTitleLen = 255;
	char title[maxTitleLen + 1];
	bool fullScreen;
	bool userResizable;
	WNDPROC baseClassProc;
	std::list<GestureRecognizer*> gestureRecognizers;

	Window(int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, int samples, char title[], bool fullScreen, bool userResizable, int x, int y, int clientWidth, int clientHeight);
	Window(int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, int samples, HWND hWnd);
	virtual void dispose();
	virtual void _free();
	virtual bool processMessages();
	virtual HWND getOSHandle();
	virtual HDC getDisplay();
	virtual int getClientWidth();
	virtual int getClientHeight();
	virtual void getClientSize(int* w, int* h);
	virtual void present();
	virtual void addGestureRecognizer(pgn::DraggingGestureRecognizer* recognizer);
	virtual void removeGestureRecognizer(pgn::GestureRecognizer* recognizer);
};

enum
{
	  WM_PGN_ACTIVATE = WM_USER
	, WM_PGN_DEACTIVATE
	, WM_PGN_CLOSE
};

class Window;
static std::map<HWND, Window*>* map = 0;

static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	struct {
		unsigned message;
		unsigned wParam;
		unsigned lParam;
	} translatedMessage;

	translatedMessage.message = 0;
	translatedMessage.wParam = 0;	// 后面给PostMessage传了这个值，若不初始化，debug版会异常
	translatedMessage.lParam = 0;	// 后面给PostMessage传了这个值，若不初始化，debug版会异常

	LRESULT result = 0;

	switch (message)
	{
	case WM_ACTIVATE:

		translatedMessage.message = wParam & 0x0000ffff ? WM_PGN_ACTIVATE : WM_PGN_DEACTIVATE;
		break;

	case WM_CLOSE:

		translatedMessage.message = WM_PGN_CLOSE;
		break;

	case WM_SETCURSOR:

		SetCursor(LoadCursor(NULL, IDC_ARROW));
		return true;

	default:

		if (map)
		{
			Window* wnd = (*map)[hWnd];

			for (auto recognizer : wnd->gestureRecognizers)
				recognizer->processInput(message, wParam, lParam);

			if (wnd->baseClassProc)
			{
				result = CallWindowProc(wnd->baseClassProc, hWnd, message, wParam, lParam);
				break;
			}
		}

		result = DefWindowProc(hWnd, message, wParam, lParam);
	}

	if (translatedMessage.message)
		PostMessage(hWnd, translatedMessage.message, translatedMessage.wParam, translatedMessage.lParam);

	return result;
}

const char wndClassName[] = "PGN Window";

void regWndClass()
{
	WNDCLASS wc =
	{
		NULL,
		wndProc,
		0, 0,
		GetModuleHandle(NULL),
		NULL, NULL,
		NULL,
		NULL,
		wndClassName
	};
	RegisterClass(&wc);
}

void unregWndClass()
{
	UnregisterClass(wndClassName, NULL);
}

static int wndClassRefCount = 0;

void createSwapChain(HDC hDC, int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, int samples)
{
	struct AttributeList
	{
		int redBitsTag;			int redBits;
		int greenBitsTag;		int greenBits;
		int blueBitsTag;		int blueBits;
		int alphaBitsTag;		int alphaBits;
		int depthBitsTag;		int depthBits;
		int stencilBitsTag;		int stencilBits;
		int sampleBuffersTag;	int sampleBuffers;
		int samplesTag;			int samples;
		int doubleBufferTag;	int doubleBuffer;
		int swapMethodTag;		int swapMethod;
		int terminator;
	};

	AttributeList attribs =
	{
		WGL_RED_BITS_ARB, 0,
		WGL_GREEN_BITS_ARB, 0,
		WGL_BLUE_BITS_ARB, 0,
		WGL_ALPHA_BITS_ARB, 0,
		WGL_DEPTH_BITS_ARB, 0,
		WGL_STENCIL_BITS_ARB, 0,
		WGL_SAMPLE_BUFFERS_ARB, 0,
		WGL_SAMPLES_ARB, 0,
		WGL_DOUBLE_BUFFER_ARB, 0,
		WGL_SWAP_METHOD_ARB, 0,
		0
	};

	attribs.redBits = redBits;
	attribs.greenBits = greenBits;
	attribs.blueBits = blueBits;
	attribs.alphaBits = alphaBits;
	attribs.depthBits = depthBits;
	attribs.stencilBits = stencilBits;
	if (samples > 1)
	{
		attribs.sampleBuffers = 1;
		attribs.samples = samples;
	}
	else
	{
		attribs.sampleBuffers = 0;
		attribs.samples = 0;
	}
	attribs.doubleBuffer = 1;
	attribs.swapMethod = WGL_SWAP_EXCHANGE_ARB;

	int formats[2];
	unsigned numFormats;
	wglChoosePixelFormatARB(hDC, (int*)&attribs, NULL, 2, formats, &numFormats);
	//assert(numFormats == 1);
	PIXELFORMATDESCRIPTOR pfd;
	SetPixelFormat(hDC, formats[0], &pfd);
}

Window::Window(int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, int samples, char title[], bool fullScreen, bool userResizable, int x, int y, int clientWidth, int clientHeight)
{
	if (wndClassRefCount++ == 0)
		regWndClass();

	strncpy(this->title, title, maxTitleLen);
	this->title[maxTitleLen] = '\0';
	this->fullScreen = fullScreen;
	this->userResizable = userResizable;

	DWORD style, exStyle;
	if(fullScreen)
	{
		exStyle = WS_EX_TOPMOST;
		style = WS_POPUP;
	}
	else
	{
		exStyle = WS_EX_APPWINDOW;
		style = userResizable ? WS_OVERLAPPEDWINDOW : WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX;
	}
	RECT clientRect = {0, 0, clientWidth, clientHeight};
	AdjustWindowRectEx(&clientRect, style, FALSE, exStyle);
	hWnd = CreateWindowEx(exStyle, wndClassName, this->title, style, x, y, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, HWND_DESKTOP, NULL, GetModuleHandle(NULL), NULL);
	hDC = GetDC(hWnd);

	baseClassProc = 0;

	if (!map)
		map = debug_new std::map<HWND, Window*>;

	(*map)[hWnd] = this;

	ShowWindow(hWnd, SW_SHOWNORMAL);

	wgl.addRef();
	createSwapChain(hDC, redBits, greenBits, blueBits, alphaBits, depthBits, stencilBits, samples);
}

Window::Window(int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, int samples, HWND hWnd)
{
	fullScreen = false;

	this->hWnd = hWnd;
	hDC = GetDC(hWnd);

	baseClassProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)wndProc);

	if (!map)
		map = debug_new std::map<HWND, Window*>;

	(*map)[hWnd] = this;

	wgl.addRef();
	createSwapChain(hDC, redBits, greenBits, blueBits, alphaBits, depthBits, stencilBits, samples);
}

void Window::dispose()
{
	// 严格来讲应该在删除hWnd之前先删除pgn::Window对象。
	// 但有些情况不太容易控制这个顺序，比如使用.net窗口的时候。
	// 所以要先判断一下hWnd是不是已经被删。
	// 现在的这个判断方法有个缺陷，如果又创建了一个窗口，句柄还一样，那就不对了。
	if (IsWindow(hWnd))
	{
		ReleaseDC(hWnd, hDC);

		char _wndClassName[1024];
		GetClassName(hWnd, _wndClassName, 1024);

		if (!strcmp(_wndClassName, wndClassName))
		{
			if (fullScreen)
				ChangeDisplaySettings(NULL, 0);

			DestroyWindow(hWnd);

			if (--wndClassRefCount == 0)
				unregWndClass();
		}
		else
		{
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)baseClassProc);
		}

		map->erase(hWnd);

		if (map->empty())
		{
			delete map;
			map = 0;
		}
	}

	wgl.release();
}

pgn::Window* pgn::Window::create(int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, int samples, char title[], int x, int y, int clientWidth, int clientHeight, bool userResizable)
{
	return debug_new ::Window(redBits, greenBits, blueBits, alphaBits, depthBits, stencilBits, samples, title, false, userResizable, x, y, clientWidth, clientHeight);
}

pgn::Window* pgn::Window::createFullScreen(int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, int samples, char title[], int w, int h)
{
	return debug_new ::Window(redBits, greenBits, blueBits, alphaBits, depthBits, stencilBits, samples, title, true, false, 0, 0, w, h);
}

pgn::Window* pgn::Window::create(int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, int samples, HWND hWnd)
{
	return debug_new ::Window(redBits, greenBits, blueBits, alphaBits, depthBits, stencilBits, samples, hWnd);
}

void Window::_free()
{
	delete this;
}

bool Window::processMessages()
{
	MSG msg;
	while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
	{
		switch(msg.message)
		{
			case WM_PGN_ACTIVATE:

				if(fullScreen)
				{
					ShowWindow(hWnd, SW_RESTORE); // 先要从最小化状态还原才能得到正确的窗口尺寸。
					DEVMODE dm;
					dm.dmSize = sizeof(DEVMODE);
					dm.dmDriverExtra = 0;
					dm.dmPelsWidth = getClientWidth();
					dm.dmPelsHeight = getClientHeight();
					dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
					ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
				}
				break;

			case WM_PGN_DEACTIVATE:

				if(fullScreen)
				{
					ChangeDisplaySettings(NULL, 0);
					ShowWindow(hWnd, SW_MINIMIZE);
				}
				break;

			case WM_PGN_CLOSE:

				return false;

			default:
				DispatchMessage(&msg);
		}
	}
	return true;
}

HWND Window::getOSHandle()
{
	return hWnd;
}

HDC Window::getDisplay()
{
	return hDC;
}

int Window::getClientWidth()
{
	RECT rc;
	GetClientRect(hWnd, &rc);
	return rc.right;
}

int Window::getClientHeight()
{
	RECT rc;
	GetClientRect(hWnd, &rc);
	return rc.bottom;
}

void Window::getClientSize(int* w, int* h)
{
	RECT rc;
	GetClientRect(hWnd, &rc);
	*w = rc.right;
	*h = rc.bottom;
}

void Window::present()
{
	SwapBuffers(hDC);
}

void Window::addGestureRecognizer(pgn::DraggingGestureRecognizer* _recognizer)
{
	DraggingGestureRecognizer* recognizer = (DraggingGestureRecognizer*)_recognizer;
	gestureRecognizers.push_back((GestureRecognizer*)recognizer);
}

void Window::removeGestureRecognizer(pgn::GestureRecognizer* recognizer)
{

}
