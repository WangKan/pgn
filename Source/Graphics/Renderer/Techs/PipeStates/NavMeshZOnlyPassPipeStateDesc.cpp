#include "Common.h"
#include "PipeStateDesc.h"

using namespace pgn;

extern "C" char navMeshZOnlyPassVS[];
extern "C" char navMeshZOnlyPassPS[];

static PipeStateDesc pipeStateDesc =
{
	navMeshZOnlyPassVS,
	navMeshZOnlyPassPS,
	0,
	0,
	twoSidedRasterizerStateDesc,
	commonDepthStencilStateDesc,
	blendingDisabledStateDesc
};

PipeStateDesc* navMeshZOnlyPassPipeStateDesc = &pipeStateDesc;
