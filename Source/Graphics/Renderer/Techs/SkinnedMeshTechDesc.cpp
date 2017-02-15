#include "../Passes/Passes.h"
#include "PipeStates/SkinnedMeshGeometryPassPipeStateDesc.h"
#include "PipeStates/SkinnedMeshShadingPassPipeStateDesc.h"
#include "PipeStates/SkinnedMeshZOnlyPassPipeStateDesc.h"
#include "TechDesc.h"

static PipeStateDesc* pipeStateDescs[numPasses];

namespace {
	class Constructor
	{
	public:
		Constructor()
		{
			pipeStateDescs[Z_PRE_PASS] = skinnedMeshZOnlyPassPipeStateDesc;
			pipeStateDescs[GEOMETRY_PASS] = skinnedMeshGeometryPassPipeStateDesc;
			pipeStateDescs[FORWARD_SHADING_PASS] = skinnedMeshShadingPassPipeStateDesc;
		}
	};

	static Constructor constructor;
}

static TechDesc techDesc =
{
	pipeStateDescs
};

TechDesc* skinnedMeshTechDesc = &techDesc;
