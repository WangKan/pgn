#include <PGN/RenderingSystem/DepthStencilState.h>
#include "Common.h"
#include "PipeStateDesc.h"

using namespace pgn;

extern "C" char skyBoxRenderingPassVS[];
extern "C" char skyBoxRenderingPassPS[];

static SamplerDesc* samplerDescs[] =
{
	  diffuseMapSamplerDesc
};

static DepthStencilStateDesc depthStencilStateDesc =
{
	true,					// depthEnable
	false,					// depthWrite
	EQUAL,					// depthFunc

	false,					// stencilEnable
	0xff,					// stencilReadMask
	0xff,					// stencilWriteMask

	//frontFace
	ALWAYS,					// stencilFunc
	KEEP,					// stencilFail
	KEEP,					// depthFail
	KEEP,					// depthPass

	//backFace
	ALWAYS,					// stencilFunc
	KEEP,					// stencilFail
	KEEP,					// depthFail
	KEEP					// depthPass
};

static PipeStateDesc pipeStateDesc =
{
	skyBoxRenderingPassVS,
	skyBoxRenderingPassPS,
	samplerDescs,
	sizeof(samplerDescs) / sizeof(samplerDescs[0]),
	commonRasterizerStateDesc,
	&depthStencilStateDesc,
	blendingDisabledStateDesc
};

PipeStateDesc* skyBoxRenderingPassPipeStateDesc = &pipeStateDesc;
