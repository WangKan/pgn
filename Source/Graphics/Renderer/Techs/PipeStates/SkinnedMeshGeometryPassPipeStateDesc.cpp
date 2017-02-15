#include "Common.h"
#include "PipeStateDesc.h"

using namespace pgn;

extern "C" char skinnedMeshGeometryPassVS[];
extern "C" char skinnedMeshGeometryPassPS[];

static SamplerDesc* samplerDescs[] =
{
	diffuseMapSamplerDesc
};

static PipeStateDesc pipeStateDesc =
{
	skinnedMeshGeometryPassVS,
	skinnedMeshGeometryPassPS,
	samplerDescs,
	sizeof(samplerDescs) / sizeof(samplerDescs[0]),
	commonRasterizerStateDesc,
	commonDepthStencilStateDesc,
	blendingDisabledStateDesc
};

PipeStateDesc* skinnedMeshGeometryPassPipeStateDesc = &pipeStateDesc;
