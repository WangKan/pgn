#include "../Passes/Passes.h"
#include "PipeStates/SkinnedMeshForwardShadingPassPipeStateDesc.h"
#include "PipeStates/SkinnedMeshGeometryPassPipeStateDesc.h"
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
			pipeStateDescs[FORWARD_SHADING_PASS] = skinnedMeshForwardShadingPassPipeStateDesc;
		}
	};

	static Constructor constructor;
}

static TechDesc techDesc =
{
	pipeStateDescs
};

TechDesc* skinnedMeshTechDesc = &techDesc;
