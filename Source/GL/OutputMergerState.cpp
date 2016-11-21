#include <PGN/Common/debug_new.h>
#include <PGN/Platform/UI/GL.h>
#include <PGN/RenderingSystem/OutputMergerState.h>
#include "RenderingSystem.h"

class OutputMergerState : public pgn::OutputMergerState
{
public:
	pgn::OutputMergerStateDesc desc;

	OutputMergerState(pgn::OutputMergerStateDesc* desc);
	virtual void dispose();
	virtual void _free();
	virtual void destroy();
};

OutputMergerState::OutputMergerState(pgn::OutputMergerStateDesc* desc)
{
	this->desc = *desc;
}

void OutputMergerState::dispose()
{
}

pgn::OutputMergerState* RenderingSystem::createOutputMergerState(pgn::OutputMergerStateDesc* desc)
{
	return debug_new OutputMergerState(desc);
}

void OutputMergerState::_free()
{
	delete this;
}

void OutputMergerState::destroy()
{
	delete this;
}

void RenderingSystem::setOutputMergerState(pgn::OutputMergerState* state/*, float blendFactor[4], float sampleCoverage*/)
{
	pgn::OutputMergerStateDesc* desc = &((OutputMergerState*)state)->desc;

	if(/*desc->alphaToCoverageEnable*/false)
	{
		glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	}
	else
	{
		glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	}

	glSampleCoverage(/*sampleCoverage*/1.0f, GL_FALSE);

	//if(blendFactor)
	//{
	//	glBlendColor(blendFactor[0], blendFactor[1], blendFactor[2], blendFactor[3]);
	//}

	for(int i = 0; i < /*pgn::maxNumRenderTargets*/1; i++)
	{
		pgn::RenderTargetBlendDesc *rt = &desc->renderTargets[i];

		if(rt->blendEnable)
		{
			static unsigned op[] =
			{
				GL_FUNC_ADD, GL_FUNC_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT, GL_MIN, GL_MAX
			};

			static unsigned factor[] =
			{
				GL_ZERO,
				GL_ONE,
				GL_SRC_COLOR,
				GL_ONE_MINUS_SRC_COLOR,
				GL_SRC_ALPHA,
				GL_ONE_MINUS_SRC_ALPHA,
				GL_DST_COLOR,
				GL_ONE_MINUS_DST_COLOR,
				GL_DST_ALPHA,
				GL_ONE_MINUS_DST_ALPHA,
				GL_CONSTANT_COLOR,
				GL_ONE_MINUS_CONSTANT_COLOR,
				GL_SRC_ALPHA_SATURATE
			};

			//glEnablei(GL_BLEND, i);
			//glBlendEquationSeparatei(i, op[rt->opRGB], op[rt->opAlpha]);
			//glBlendFuncSeparatei(i, factor[rt->factorSrcRGB], factor[rt->factorDestRGB], factor[rt->factorSrcAlpha], factor[rt->factorDestAlpha]);
			glEnable(GL_BLEND);
			glBlendEquationSeparate(op[rt->opRGB], op[rt->opAlpha]);
			glBlendFuncSeparate(factor[rt->factorSrcRGB], factor[rt->factorDestRGB], factor[rt->factorSrcAlpha], factor[rt->factorDestAlpha]);
		}
		else
		{
			//glDisablei(GL_BLEND, i);
			glDisable(GL_BLEND);
		}

		unsigned mask = rt->writeMask;
		glColorMask(mask & pgn::RED, mask & pgn::GREEN, mask & pgn::BLUE, mask & pgn::ALPHA);
	}
}
