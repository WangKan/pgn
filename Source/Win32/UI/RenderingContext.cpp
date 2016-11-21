#define PGN_DLL_EXPORT
#include <PGN/Platform/UI/RenderingContext.h>
#undef PGN_DLL_EXPORT

#include <gl/gl.h>
#include <gl/wglext.h>
#include <PGN/Common/debug_new.h>
#include <vector>
#include "WGL.h"

extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

class RenderingContext : public pgn::RenderingContext
{
	std::vector<HGLRC> rcSet;
public:
	RenderingContext(HDC hDC, int numThreads);
	virtual void dispose();
	virtual void _free();
	virtual void beginDraw(HDC hDC, int thread);
	virtual void endDraw();
	virtual void beginFrame();
};

RenderingContext::RenderingContext(HDC hDC, int numThreads)
{
	wgl.addRef();

	for (int i = 0; i < numThreads; i++)
		rcSet.push_back(wglCreateContext(hDC));

	for (int i = 0; i < numThreads - 1; i++)
		wglShareLists(rcSet[i], rcSet[i + 1]);
}

void RenderingContext::dispose()
{
	for (auto rc : rcSet)
		wglDeleteContext(rc);

	wgl.release();
}

pgn::RenderingContext* pgn::RenderingContext::create(HDC hDC, int numThreads)
{
	return debug_new ::RenderingContext(hDC, numThreads);
}

void RenderingContext::_free()
{
	delete this;
}

void RenderingContext::beginDraw(HDC hDC, int thread)
{
	wglMakeCurrent(hDC, rcSet[thread]);
	wglSwapIntervalEXT(0);
}

void RenderingContext::endDraw()
{
	glFinish();
	wglMakeCurrent(NULL, NULL);
}

void RenderingContext::beginFrame()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	unsigned backBuf = GL_BACK_LEFT;
	glDrawBuffers(1, &backBuf);
}
