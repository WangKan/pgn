#include "Common.h"
#include "PipeStateDesc.h"

using namespace pgn;

extern "C" char staticMeshForwardShadingPassVS[];
extern "C" char staticMeshForwardShadingPassPS[];

static SamplerDesc* samplerDescs[] =
{
	  diffuseMapSamplerDesc
	, pointSamplerDesc
};

static PipeStateDesc pipeStateDesc =
{
	staticMeshForwardShadingPassVS,
	staticMeshForwardShadingPassPS,
	samplerDescs,
	sizeof(samplerDescs) / sizeof(samplerDescs[0]),
	commonRasterizerStateDesc,
	commonDepthStencilStateDesc,
	blendingDisabledStateDesc
};

PipeStateDesc* staticMeshForwardShadingPassPipeStateDesc = &pipeStateDesc;
