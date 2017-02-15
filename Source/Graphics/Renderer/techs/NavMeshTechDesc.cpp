#include "../passes/passes.h"
#include "PipeStates/NavMeshShadingPassPipeStateDesc.h"
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
			pipeStateDescs[TRANSPARENCY_FORWARD_SHADING_PASS] = navMeshShadingPassPipeStateDesc;
		}
	};

	static Constructor constructor;
}

static TechDesc techDesc =
{
	pipeStateDescs
};

TechDesc* navMeshTechDesc = &techDesc;
