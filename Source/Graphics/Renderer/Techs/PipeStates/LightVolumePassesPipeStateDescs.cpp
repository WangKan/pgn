#include <PGN/RenderingSystem/DepthStencilState.h>
#include <PGN/RenderingSystem/OutputMergerState.h>
#include "Common.h"
#include "PipeStateDesc.h"

using namespace pgn;

extern "C" char lightVolumePassesVS[];
extern "C" char lightVolumePassesPS[];

static DepthStencilStateDesc depthStencilStateDesc =
{
	true,					// depthEnable
	false,					// depthWrite
	LESS,					// depthFunc

	true,					// stencilEnable
	0xff,					// stencilReadMask
	0xff,					// stencilWriteMask

	//frontFace
	ALWAYS,					// stencilFunc
	KEEP,					// stencilFail
	DECR_SAT,				// depthFail
	KEEP,					// depthPass

	//backFace
	ALWAYS,					// stencilFunc
	KEEP,					// stencilFail
	INCR_SAT,				// depthFail
	KEEP					// depthPass
};

static OutputMergerStateDesc outputMergerStateDesc =
{
	//false,					// alphaToCoverageEnable

	//renderTargets[0]
	false,					// blendEnable
	ADD,					// opRGB
	INV_DEST_ALPHA,			// factorSrcRGB
	DEST_ALPHA,				// factorDestRGB
	ADD,					// opAlpha
	ZERO,					// factorSrcAlpha
	ONE,					// factorDestAlpha

	0						// writeMask
};

static PipeStateDesc pass1 =
{
	lightVolumePassesVS,
	lightVolumePassesPS,
	0,
	0,
	backFaceRasterizerStateDesc,
	&depthStencilStateDesc,
	&outputMergerStateDesc
};

static PipeStateDesc pass2 =
{
	lightVolumePassesVS,
	lightVolumePassesPS,
	0,
	0,
	commonRasterizerStateDesc,
	&depthStencilStateDesc,
	&outputMergerStateDesc
};

PipeStateDesc* lightVolumeBackFacePassPipeStateDesc = &pass1;
PipeStateDesc* lightVolumeFrontFacePassPipeStateDesc = &pass2;
