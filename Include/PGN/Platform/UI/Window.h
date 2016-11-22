#pragma once
#include "../../Common/DllInterface.h"
#include "../../Common/Interface.h"
#include "Display.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef __APPLE__
#include <TargetConditionals.h>
#ifdef TARGET_OS_IPHONE
@class UIView;
#endif
#endif

namespace pgn {

#ifdef _WIN32
typedef HWND OSWindow;
#endif

#ifdef __APPLE__
#include <TargetConditionals.h>
#ifdef TARGET_OS_IPHONE
typedef UIView* OSWindow;
#endif
#endif

#ifdef __psp2__
typedef void* OSWindow;
#endif

class DraggingGestureRecognizer;
class GestureRecognizer;

class Window : public Interface
{
public:
	static PGN_DLL_INTERFACE Window* create				(int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, int samples, char title[], int x, int y, int clientWidth, int clientHeight, bool userResizable = true);
	static PGN_DLL_INTERFACE Window* createFullScreen	(int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, int samples, char title[], int w, int h);
	static PGN_DLL_INTERFACE Window* create				(int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, int samples, OSWindow wnd);
	virtual bool processMessages() = 0;
	virtual OSWindow getOSHandle() = 0;
	virtual Display getDisplay() = 0; // ¿Í»§Çø
	virtual int getClientWidth() = 0;
	virtual int getClientHeight() = 0;
	virtual void getClientSize(int* w, int* h) = 0;
	virtual void present() = 0;
	virtual void addGestureRecognizer(DraggingGestureRecognizer* recognizer) = 0;
	virtual void removeGestureRecognizer(GestureRecognizer* recognizer) = 0;
};

}
