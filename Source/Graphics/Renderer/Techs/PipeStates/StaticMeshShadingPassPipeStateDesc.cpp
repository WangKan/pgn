#include "Common.h"
#include "PipeStateDesc.h"

using namespace pgn;

extern "C" char staticMeshShadingPassVS[];
extern "C" char staticMeshShadingPassPS[];

static SamplerDesc* samplerDescs[] =
{
	  diffuseMapSamplerDesc
	, pointSamplerDesc
};

static PipeStateDesc pipeStateDesc =
{
	staticMeshShadingPassVS,
	staticMeshShadingPassPS,
	samplerDescs,
	sizeof(samplerDescs) / sizeof(samplerDescs[0]),
	commonRasterizerStateDesc,
	commonDepthStencilStateDesc,
	blendingDisabledStateDesc
};

PipeStateDesc* staticMeshShadingPassPipeStateDesc = &pipeStateDesc;
