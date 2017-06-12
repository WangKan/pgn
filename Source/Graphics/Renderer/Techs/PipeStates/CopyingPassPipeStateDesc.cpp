#include "Common.h"
#include "PipeStateDesc.h"

using namespace pgn;

extern "C" char copyingPassVS[];
extern "C" char copyingPassPS[];

static SamplerDesc* samplerDescs[] =
{
	pointSamplerDesc
};

static PipeStateDesc pipeStateDesc =
{
	copyingPassVS,
	copyingPassPS,
	samplerDescs,
	sizeof(samplerDescs) / sizeof(samplerDescs[0]),
	commonRasterizerStateDesc,
	depthStencilTestingDisabledStateDesc,
	blendingDisabledStateDesc
};

PipeStateDesc* copyingPassPipeStateDesc = &pipeStateDesc;
