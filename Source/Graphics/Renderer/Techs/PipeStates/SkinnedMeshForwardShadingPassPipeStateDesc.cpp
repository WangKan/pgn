#include "Common.h"
#include "PipeStateDesc.h"

using namespace pgn;

extern "C" char skinnedMeshForwardShadingPassVS[];
extern "C" char skinnedMeshForwardShadingPassPS[];

static SamplerDesc* samplerDescs[] =
{
	  diffuseMapSamplerDesc
	, pointSamplerDesc
};

static PipeStateDesc pipeStateDesc =
{
	skinnedMeshForwardShadingPassVS,
	skinnedMeshForwardShadingPassPS,
	samplerDescs,
	sizeof(samplerDescs) / sizeof(samplerDescs[0]),
	commonRasterizerStateDesc,
	commonDepthStencilStateDesc,
	blendingDisabledStateDesc
};

PipeStateDesc* skinnedMeshForwardShadingPassPipeStateDesc = &pipeStateDesc;
