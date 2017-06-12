#include "../Passes/Passes.h"
#include "PipeStates/CopyingPassPipeStateDesc.h"
#include "PipeStates/LightingPassPipeStateDesc.h"
#include "TechDesc.h"

static PipeStateDesc* pipeStateDescs[numPasses];

namespace {
	class Constructor
	{
	public:
		Constructor()
		{
			pipeStateDescs[LIGHTING_PASS] = lightingPassPipeStateDesc;
			pipeStateDescs[COPYING_PASS] = copyingPassPipeStateDesc;
		}
	};

	static Constructor constructor;
}

static TechDesc techDesc =
{
	pipeStateDescs
};

TechDesc* postProcessingTechDesc = &techDesc;
