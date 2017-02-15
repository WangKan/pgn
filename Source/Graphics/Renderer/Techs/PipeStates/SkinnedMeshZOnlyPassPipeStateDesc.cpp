#include "Common.h"
#include "PipeStateDesc.h"

using namespace pgn;

extern "C" char skinnedMeshZOnlyPassVS[];
extern "C" char skinnedMeshZOnlyPassPS[];

static PipeStateDesc pipeStateDesc =
{
	skinnedMeshZOnlyPassVS,
	skinnedMeshZOnlyPassPS,
	0,
	0,
	commonRasterizerStateDesc,
	commonDepthStencilStateDesc,
	blendingDisabledStateDesc
};

PipeStateDesc* skinnedMeshZOnlyPassPipeStateDesc = &pipeStateDesc;
