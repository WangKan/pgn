#include "Common.h"
#include "PipeStateDesc.h"

using namespace pgn;

extern "C" char lightingPassVS[];
extern "C" char lightingPassPS[];

static SamplerDesc* samplerDescs[] =
{
	  pointSamplerDesc
	, pointSamplerDesc
	, pointSamplerDesc
	, pointSamplerDesc
};

static PipeStateDesc pipeStateDesc =
{
	lightingPassVS,
	lightingPassPS,
	samplerDescs,
	sizeof(samplerDescs) / sizeof(samplerDescs[0]),
	commonRasterizerStateDesc,
	depthStencilTestingDisabledStateDesc,
	blendingDisabledStateDesc
};

PipeStateDesc* lightingPassPipeStateDesc = &pipeStateDesc;
