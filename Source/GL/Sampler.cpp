#include <PGN/Common/debug_new.h>
#include <PGN/Platform/UI/GL.h>
#include <PGN/RenderingSystem/Sampler.h>
#include "CmpFunc.h"
#include "Program.h"
#include "RenderingSystem.h"

class Sampler : public pgn::Sampler
{
public:
	unsigned sampler;

	Sampler(pgn::SamplerDesc* desc);
	virtual void dispose();
	virtual void _free();
};

const unsigned minFilter[] = { GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR };
const unsigned magFilter[] = { GL_NEAREST, GL_LINEAR };
const unsigned wrap[] = { GL_CLAMP_TO_EDGE, GL_REPEAT, GL_MIRRORED_REPEAT };

Sampler::Sampler(pgn::SamplerDesc* desc)
{
	glGenSamplers(1, &sampler);
	glBindSampler(0, sampler);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, minFilter[desc->minFilter]);
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, magFilter[desc->magFilter]);
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, wrap[desc->wrapU]);
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, wrap[desc->wrapV]);
	glSamplerParameteri(sampler, GL_TEXTURE_COMPARE_FUNC, cmpFunc[desc->cmpFunc]);
	if(pgn::getGLInfo()->EXT_texture_filter_anisotropic)
	{
		glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, desc->maxAniso);
	}
}

void Sampler::dispose()
{
	glDeleteSamplers(1, &sampler);
}

pgn::Sampler* RenderingSystem::createSampler(pgn::SamplerDesc* desc)
{
	return debug_new Sampler(desc);
}

void Sampler::_free()
{
	delete this;
}

void RenderingSystem::setSamplers(int startSlot, int numSamplers, pgn::Sampler* samplers[])
{
	for(int i = 0; i < numSamplers; i++)
	{
		int indexTextureUnit = startSlot + i;
		glBindSampler(indexTextureUnit, ((Sampler*)samplers[i])->sampler);
	}
}
