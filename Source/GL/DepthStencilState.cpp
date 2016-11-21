#include <PGN/Common/debug_new.h>
#include <PGN/Platform/UI/GL.h>
#include <PGN/RenderingSystem/DepthStencilState.h>
#include "CmpFunc.h"
#include "RenderingSystem.h"

class DepthStencilState : public pgn::DepthStencilState
{
public:
	pgn::DepthStencilStateDesc desc;

	DepthStencilState(pgn::DepthStencilStateDesc* desc);
	virtual void dispose();
	virtual void _free();
	virtual void destroy();
};

DepthStencilState::DepthStencilState(pgn::DepthStencilStateDesc* desc)
{
	this->desc = *desc;
}

void DepthStencilState::dispose()
{
}

pgn::DepthStencilState* RenderingSystem::createDepthStencilState(pgn::DepthStencilStateDesc* desc)
{
	return debug_new DepthStencilState(desc);
}

void DepthStencilState::_free()
{
	delete this;
}

void DepthStencilState::destroy()
{
	delete this;
}

const unsigned stencilOp[] =
{
	GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR, GL_DECR, GL_INVERT, GL_INCR_WRAP, GL_DECR_WRAP
};

void RenderingSystem::setDepthStencilState(pgn::DepthStencilState* state, unsigned stencilRef)
{
	pgn::DepthStencilStateDesc* desc = &((DepthStencilState*)state)->desc;

	if(desc->stencilEnable)
	{
		glEnable(GL_STENCIL_TEST);
		glStencilFuncSeparate(GL_FRONT, cmpFunc[desc->frontFace.stencilFunc], stencilRef, desc->stencilReadMask);
		glStencilFuncSeparate(GL_BACK, cmpFunc[desc->backFace.stencilFunc], stencilRef, desc->stencilReadMask);
	}
	else
	{
		glDisable(GL_STENCIL_TEST);
	}

	if(desc->depthEnable)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(cmpFunc[desc->depthFunc]);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}

	glStencilOpSeparate(GL_FRONT, stencilOp[desc->frontFace.stencilFail], stencilOp[desc->frontFace.depthFail], stencilOp[desc->frontFace.depthPass]);
	glStencilOpSeparate(GL_BACK, stencilOp[desc->backFace.stencilFail], stencilOp[desc->backFace.depthFail], stencilOp[desc->backFace.depthPass]);
	glStencilMask(desc->stencilWriteMask);
	glDepthMask(desc->depthWrite);
}
