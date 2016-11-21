#include <PGN/Common/debug_new.h>
#include <PGN/Platform/UI/Window.h>
#include "CAEAGLLayerEX.h"
#include "GLInfo.h"

@interface GLView : UIView

+ (Class) layerClass;

@end

@implementation GLView

+ (Class) layerClass
{
    return [CAEAGLLayerEX class];
}

@end

class Window : public pgn::Window
{
	GLView* view;
    CAEAGLLayerEX* layer;
public:
    void init(int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, int samples, CGRect rect);
	Window(int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, int samples, int x, int y, int w, int h);
	Window(int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, int samples, UIView* view);
	virtual void dispose();
	virtual void _free();
	virtual bool processMessages();
	virtual UIView* getOSHandle();
	virtual CAEAGLLayer* getDisplay();
	virtual int getClientWidth();
	virtual int getClientHeight();
	virtual void getClientSize(int* w, int* h);
	virtual void present();
};

void Window::init(int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, int samples, CGRect rect)
{
    view = [[GLView alloc] initWithFrame:rect];
    //view.opaque = YES;

    layer = (CAEAGLLayerEX*)view.layer;
    layer.contentsScale = [UIScreen mainScreen].scale;
    layer.drawableProperties = @
    {
          kEAGLDrawablePropertyColorFormat        : kEAGLColorFormatRGBA8
        , kEAGLDrawablePropertyRetainedBacking    : @NO
    };
    layer.depthBits = depthBits;
    layer.stencilBits = stencilBits;

    glInfo.addRef();
}

Window::Window(int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, int samples, int x, int y, int w, int h)
{
    CGRect rect = {(CGFloat)x, (CGFloat)y, (CGFloat)w, (CGFloat)h};
    init(redBits, greenBits, blueBits, alphaBits, depthBits, stencilBits, samples, rect);
}

Window::Window(int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, int samples, UIView* parent)
{
    init(redBits, greenBits, blueBits, alphaBits, depthBits, stencilBits, samples, parent.bounds);
    [parent addSubview:view];
}

void Window::dispose()
{
    glInfo.release();
    [view removeFromSuperview];
}

pgn::Window* pgn::Window::create(int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, int samples, char title[], int x, int y, int clientWidth, int clientHeight, bool userResizable)
{
    return debug_new ::Window(redBits, greenBits, blueBits, alphaBits, depthBits, stencilBits, samples, x, y, clientWidth, clientHeight);
}

pgn::Window* pgn::Window::createFullScreen(int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, int samples, char title[], int w, int h)
{
	return debug_new ::Window(redBits, greenBits, blueBits, alphaBits, depthBits, stencilBits, samples, 0, 0, w, h);
}

pgn::Window* pgn::Window::create(int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, int samples, UIView* view)
{
	return debug_new ::Window(redBits, greenBits, blueBits, alphaBits, depthBits, stencilBits, samples, view);
}

void Window::_free()
{
	delete this;
}

bool Window::processMessages()
{
    return true;
}

UIView* Window::getOSHandle()
{
	return view;
}

CAEAGLLayer* Window::getDisplay()
{
	return layer;
}

int Window::getClientWidth()
{
    return view.bounds.size.width * [UIScreen mainScreen].scale;
}

int Window::getClientHeight()
{
    return view.bounds.size.height * [UIScreen mainScreen].scale;
}

void Window::getClientSize(int* w, int* h)
{
    *w = view.bounds.size.width * [UIScreen mainScreen].scale;
    *h = view.bounds.size.height * [UIScreen mainScreen].scale;
}

void Window::present()
{
    glBindRenderbuffer(GL_RENDERBUFFER, layer.colorBuf);
	[[EAGLContext currentContext] presentRenderbuffer:GL_RENDERBUFFER];
}
