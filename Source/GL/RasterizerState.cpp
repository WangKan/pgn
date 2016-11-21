#include <PGN/Common/debug_new.h>
#include <PGN/Platform/UI/GL.h>
#include <PGN/RenderingSystem/RasterizerState.h>
#include "RenderingSystem.h"

class RasterizerState : public pgn::RasterizerState
{
public:
	pgn::RasterizerStateDesc desc;

	RasterizerState(pgn::RasterizerStateDesc* desc);
	virtual void dispose();
	virtual void _free();
	virtual void destroy();
};

RasterizerState::RasterizerState(pgn::RasterizerStateDesc* desc)
{
	this->desc = *desc;
}

void RasterizerState::dispose()
{
}

pgn::RasterizerState* RenderingSystem::createRasterizerState(pgn::RasterizerStateDesc* desc)
{
	return debug_new RasterizerState(desc);
}

void RasterizerState::_free()
{
	delete this;
}

void RasterizerState::destroy()
{
	delete this;
}

const unsigned frontFace[] =
{
	GL_CCW, GL_CW
};

const unsigned cullFace[] =
{
	0, GL_BACK, GL_FRONT
};

void RenderingSystem::setRasterizerState(pgn::RasterizerState* state)
{
	pgn::RasterizerStateDesc* desc = &((RasterizerState*)state)->desc;

	glFrontFace(frontFace[desc->frontFace]);

	if(desc->cullFace != pgn::NONE)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(cullFace[desc->cullFace]);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}
