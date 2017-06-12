#include "../Passes/Passes.h"
#include "PipeStates/SkyBoxRenderingPassPipeStateDesc.h"
#include "TechDesc.h"

static PipeStateDesc* pipeStateDescs[numPasses];

namespace {
	class Constructor
	{
	public:
		Constructor()
		{
			pipeStateDescs[SKY_BOX_RENDERING_PASS] = skyBoxRenderingPassPipeStateDesc;
		}
	};

	static Constructor constructor;
}

static TechDesc techDesc =
{
	pipeStateDescs
};

TechDesc* skyBoxTechDesc = &techDesc;
