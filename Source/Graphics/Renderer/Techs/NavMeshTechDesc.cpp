#include "../Passes/Passes.h"
#include "PipeStates/NavMeshForwardShadingPassPipeStateDesc.h"
#include "PipeStates/NavMeshZOnlyPassPipeStateDesc.h"
#include "TechDesc.h"

static PipeStateDesc* pipeStateDescs[numPasses];

namespace {
	class Constructor
	{
	public:
		Constructor()
		{
			pipeStateDescs[TRANSPARENCY_Z_PRE_PASS] = navMeshZOnlyPassPipeStateDesc;
			pipeStateDescs[TRANSPARENCY_SHADING_PASS] = navMeshForwardShadingPassPipeStateDesc;
		}
	};

	static Constructor constructor;
}

static TechDesc techDesc =
{
	pipeStateDescs
};

TechDesc* navMeshTechDesc = &techDesc;
