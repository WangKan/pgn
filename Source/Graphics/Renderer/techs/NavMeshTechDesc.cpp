#include "../passes/passes.h"
#include "PipeStates/NavMeshShadingPassPipeStateDesc.h"
#include "TechDesc.h"

static PipeStateDesc* pipeStateDescs[numPasses];

namespace {
	class Constructor
	{
	public:
		Constructor()
		{
			pipeStateDescs[FORWARD_SHADING_PASS] = navMeshShadingPassPipeStateDesc;
		}
	};

	static Constructor constructor;
}

static TechDesc techDesc =
{
	pipeStateDescs
};

TechDesc* navMeshTechDesc = &techDesc;
