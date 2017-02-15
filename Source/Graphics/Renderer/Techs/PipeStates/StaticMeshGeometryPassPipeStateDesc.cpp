#include "Common.h"
#include "PipeStateDesc.h"

using namespace pgn;

extern "C" char staticMeshGeometryPassVS[];
extern "C" char staticMeshGeometryPassPS[];

static SamplerDesc* samplerDescs[] =
{
	diffuseMapSamplerDesc
};

static PipeStateDesc pipeStateDesc =
{
	staticMeshGeometryPassVS,
	staticMeshGeometryPassPS,
	samplerDescs,
	sizeof(samplerDescs) / sizeof(samplerDescs[0]),
	commonRasterizerStateDesc,
	commonDepthStencilStateDesc,
	blendingDisabledStateDesc
};

PipeStateDesc* staticMeshGeometryPassPipeStateDesc = &pipeStateDesc;
