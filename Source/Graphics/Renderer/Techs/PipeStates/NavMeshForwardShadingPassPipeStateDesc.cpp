#include "Common.h"
#include "PipeStateDesc.h"

using namespace pgn;

extern "C" char navMeshForwardShadingPassVS[];
extern "C" char navMeshForwardShadingPassPS[];

static PipeStateDesc pipeStateDesc =
{
	navMeshForwardShadingPassVS,
	navMeshForwardShadingPassPS,
	0,
	0,
	twoSidedRasterizerStateDesc,
	commonDepthStencilStateDesc,
	alphaBlendingStateDesc
};

PipeStateDesc* navMeshForwardShadingPassPipeStateDesc = &pipeStateDesc;
