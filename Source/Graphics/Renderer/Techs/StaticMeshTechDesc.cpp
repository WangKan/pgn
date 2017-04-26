#include "../Passes/Passes.h"
#include "PipeStates/StaticMeshForwardShadingPassPipeStateDesc.h"
#include "PipeStates/StaticMeshGeometryPassPipeStateDesc.h"
#include "PipeStates/StaticMeshZOnlyPassPipeStateDesc.h"
#include "TechDesc.h"

static PipeStateDesc* pipeStateDescs[numPasses];

namespace {
	class Constructor
	{
	public:
		Constructor()
		{
			pipeStateDescs[Z_PRE_PASS] = staticMeshZOnlyPassPipeStateDesc;
			pipeStateDescs[GEOMETRY_PASS] = staticMeshGeometryPassPipeStateDesc;
			pipeStateDescs[FORWARD_SHADING_PASS] = staticMeshForwardShadingPassPipeStateDesc;
		}
	};

	static Constructor constructor;
}

static TechDesc techDesc =
{
	pipeStateDescs
};

TechDesc* staticMeshTechDesc = &techDesc;
