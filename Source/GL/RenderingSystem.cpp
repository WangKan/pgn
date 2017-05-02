#include <cassert>
#include <PGN/Common/debug_new.h>
#include <PGN/Platform/UI/GL.h>
#include <PGN/Platform/UI/RenderingContext.h>
#include "RenderingSystem.h"
#include "ResourceView.h"

RenderingSystem::RenderingSystem(pgn::RenderingContext* rc)
{
	this->rc = rc;
	offscreenFB = 0;
}

void RenderingSystem::dispose()
{
	if (offscreenFB)
		glDeleteFramebuffers(1, &offscreenFB);
}

pgn::RenderingSystem* pgn::RenderingSystem::create(RenderingContext* rc)
{
	return debug_new ::RenderingSystem(rc);
}

void RenderingSystem::_free()
{
	delete this;
}

void RenderingSystem::beginOffscreenPass(int numRenderTargets, pgn::RenderTargetView* renderTargets[], pgn::DepthStencilView* depthStencilView)
{
	if (!offscreenFB)
		glGenFramebuffers(1, &offscreenFB);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, offscreenFB);

	if (numRenderTargets)
	{
		if (((RenderTargetView*)renderTargets[0])->type == RENDER_BUFFER)
		{
			for (int i = 0; i < numRenderTargets; i++)
				glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, ((RenderTargetView*)renderTargets[i])->rb);
		}
		else// if (((RenderTargetView*)renderTargets[0])->type == TEXTURE)
		{
			for (int i = 0; i < numRenderTargets; i++)
				glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, ((RenderTargetView*)renderTargets[i])->tex, 0);
		}
	}

	static unsigned drawBufs[] =
	{
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3
	};

	glDrawBuffers(numRenderTargets, drawBufs);

	for (int i = numRenderTargets; i < pgn::maxNumRenderTargets; i++)
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, 0);

	if (depthStencilView)
	{
		if (((DepthStencilView*)depthStencilView)->type == RENDER_BUFFER)
		{
			glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, ((DepthStencilView*)depthStencilView)->rb);
		}
		else
		{
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, ((DepthStencilView*)depthStencilView)->tex, 0);
		}
	}
	else
	{
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
	}

	unsigned rrrr = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	assert(rrrr == GL_FRAMEBUFFER_COMPLETE);
}

void RenderingSystem::beginOnscreenPass()
{
	rc->beginOnscreenPass();
}

void RenderingSystem::clearDepthStencilView(pgn::DepthStencilView* depthStencilView, bool clearDepth, float depth, bool clearStencil, unsigned char stencil)
{
	if(depthStencilView)
		beginOffscreenPass(0, 0, depthStencilView);
	else
		beginOnscreenPass();

	unsigned clearMask = 0;
	if (clearDepth) clearMask |= GL_DEPTH_BUFFER_BIT;
	if (clearStencil) clearMask |= GL_STENCIL_BUFFER_BIT;

	glClearDepthf(depth);
	glClearStencil(stencil);
	glDepthMask(GL_TRUE);
	glStencilMask(0xff);
	glClear(clearMask);
	//endPass(0);
}

void RenderingSystem::clearRenderTargetView(pgn::RenderTargetView* renderTargetView, float r, float g, float b, float a)
{
	if(renderTargetView)
	{
		beginOffscreenPass(1, &renderTargetView, 0);
	}
	else
	{
		beginOnscreenPass();
	}
	glClearColor(r, g, b, a);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT);
	//endPass(0);
}

const unsigned modes[] = {GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES, GL_TRIANGLE_STRIP};

void RenderingSystem::draw(int indicesPerInstance, int instanceCount)
{
	glDrawElementsInstanced(modes[primType], indicesPerInstance, GL_UNSIGNED_SHORT, (void*)indexBufferOffset, instanceCount);
}

void RenderingSystem::endPass(pgn::SyncPoint* sync)
{
	if (sync)
		addSyncPoint((SyncPoint*)sync);
}

void RenderingSystem::finish()
{
	glFinish();
}

void RenderingSystem::flush()
{
	glFlush();
}

int RenderingSystem::getConstantBufferOffsetAlignment()
{
	int alignment;
	glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignment);
	return alignment;
}

void RenderingSystem::setPrimitiveType(pgn::PrimType primType)
{
	this->primType = primType;
}

void RenderingSystem::setViewport(int left, int top, int width, int height, int fullHeight, float minDepth, float maxDepth)
{
	glViewport(left, fullHeight-height-top, width, height);
	glDepthRangef(minDepth, maxDepth);
}
