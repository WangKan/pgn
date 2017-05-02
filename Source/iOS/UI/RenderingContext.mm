#import  <OpenGLES/ES3/gl.h>
#import  <OpenGLES/ES3/glext.h>
#include <PGN/Common/debug_new.h>
#include <PGN/Platform/UI/RenderingContext.h>
#include <vector>
#include "CAEAGLLayerEX.h"

class RenderingContext : public pgn::RenderingContext
{
	std::vector<EAGLContext*> rc;
    unsigned screenFB;
    unsigned colorBuf;
    unsigned depthStencilBuf;
public:
	RenderingContext(int numThreads);
	virtual void dispose();
	virtual void _free();
	virtual void beginDraw(CAEAGLLayer* eaglLayer, int thread);
	virtual void endDraw();
	virtual void beginOnscreenPass();
};

RenderingContext::RenderingContext(int numThreads)
{
    rc.push_back([[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3]);

	for (int i = 1; i < numThreads; i++)
		rc.push_back([[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3 sharegroup:rc[0].sharegroup]);

    [EAGLContext setCurrentContext:rc[0]];
    glGenFramebuffers(1, &screenFB);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, screenFB);
    [EAGLContext setCurrentContext:nil];

    colorBuf = 0;
}

void RenderingContext::dispose()
{
    glDeleteFramebuffers(1, &screenFB);
}

pgn::RenderingContext* pgn::RenderingContext::create(CAEAGLLayer* eaglLayer, int numThreads)
{
	return debug_new ::RenderingContext(numThreads);
}

void RenderingContext::_free()
{
	delete this;
}

void RenderingContext::beginDraw(CAEAGLLayer* eaglLayer, int thread)
{
    CAEAGLLayerEX* layer = (CAEAGLLayerEX*)eaglLayer;

    unsigned depthStencilFormat = 0;

    if(layer.stencilBits > 0)
        depthStencilFormat = GL_DEPTH24_STENCIL8;
    else if(layer.depthBits > 16)
        depthStencilFormat = GL_DEPTH_COMPONENT24;
    else if(layer.depthBits > 0)
        depthStencilFormat = GL_DEPTH_COMPONENT16;

    [EAGLContext setCurrentContext:rc[thread]];

    if (!colorBuf)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, screenFB);

        glGenRenderbuffers(1, &colorBuf);
        glBindRenderbuffer(GL_RENDERBUFFER, colorBuf);
        [rc[thread] renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorBuf);
        layer.colorBuf = colorBuf;

        if(depthStencilFormat)
        {
            int w, h;
            glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &w);
            glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &h);

            glGenRenderbuffers(1, &depthStencilBuf);
            glBindRenderbuffer(GL_RENDERBUFFER, depthStencilBuf);
            glRenderbufferStorage(GL_RENDERBUFFER, depthStencilFormat, w, h);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthStencilBuf);
        }
        else
        {
            depthStencilBuf = 0;
        }
    }
}

void RenderingContext::endDraw()
{
    glDeleteRenderbuffers(1, &colorBuf);
    if(depthStencilBuf) glDeleteRenderbuffers(1, &depthStencilBuf);
    [EAGLContext setCurrentContext:nil];
}

void RenderingContext::beginOnscreenPass()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, screenFB);
	const unsigned backBuf = GL_BACK;
	glDrawBuffers(1, &backBuf);
}
