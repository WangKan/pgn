#include "Common.h"
#include "PipeStateDesc.h"

using namespace pgn;

extern "C" char skinnedMeshShadingPassVS[];
extern "C" char skinnedMeshShadingPassPS[];

static SamplerDesc* samplerDescs[] =
{
	  diffuseMapSamplerDesc
	, pointSamplerDesc
};

static PipeStateDesc pipeStateDesc =
{
	skinnedMeshShadingPassVS,
	skinnedMeshShadingPassPS,
	samplerDescs,
	sizeof(samplerDescs) / sizeof(samplerDescs[0]),
	commonRasterizerStateDesc,
	commonDepthStencilStateDesc,
	blendingDisabledStateDesc
};

PipeStateDesc* skinnedMeshShadingPassPipeStateDesc = &pipeStateDesc;
