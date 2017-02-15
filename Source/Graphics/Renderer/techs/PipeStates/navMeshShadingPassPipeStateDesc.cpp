#include "common.h"
#include "PipeStateDesc.h"

using namespace pgn;

extern "C" char navMeshShadingPassVS[];
extern "C" char navMeshShadingPassPS[];

static PipeStateDesc pipeStateDesc =
{
	navMeshShadingPassVS,
	navMeshShadingPassPS,
	0,
	0,
	twoSidedRasterizerStateDesc,
	commonDepthStencilStateDesc,
	alphaBlendingStateDesc
};

PipeStateDesc* navMeshShadingPassPipeStateDesc = &pipeStateDesc;
