#include "Common.h"
#include "PipeStateDesc.h"

using namespace pgn;

extern "C" char staticMeshZOnlyPassVS[];
extern "C" char staticMeshZOnlyPassPS[];

static PipeStateDesc pipeStateDesc =
{
	staticMeshZOnlyPassVS,
	staticMeshZOnlyPassPS,
	0,
	0,
	commonRasterizerStateDesc,
	commonDepthStencilStateDesc,
	blendingDisabledStateDesc
};

PipeStateDesc* staticMeshZOnlyPassPipeStateDesc = &pipeStateDesc;
