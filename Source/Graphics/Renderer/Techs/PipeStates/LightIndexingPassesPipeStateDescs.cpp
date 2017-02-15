#include <PGN/RenderingSystem/DepthStencilState.h>
#include <PGN/RenderingSystem/OutputMergerState.h>
#include "Common.h"
#include "PipeStateDesc.h"

using namespace pgn;

extern "C" char lightIndexingPassesVS[];
extern "C" char lightIndexingPassesPS[];

static DepthStencilStateDesc depthStencilStateDesc =
{
	false,					// depthEnable
	false,					// depthWrite
	ALWAYS,					// depthFunc

	true,					// stencilEnable
	0xff,					// stencilReadMask
	0xff,					// stencilWriteMask

	//frontFace
	ALWAYS,					// stencilFunc
	KEEP,					// stencilFail
	KEEP,					// depthFail
	KEEP,					// depthPass

	//backFace
	NOT_EQUAL,				// stencilFunc
	KEEP,					// stencilFail
	KEEP,					// depthFail
	KEEP					// depthPass
};

static OutputMergerStateDesc outputMergerStatePass1 =
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

	RED						// writeMask
};

static OutputMergerStateDesc outputMergerStatePass2 =
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

	GREEN					// writeMask
};

static OutputMergerStateDesc outputMergerStatePass3 =
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

	BLUE					// writeMask
};

static OutputMergerStateDesc outputMergerStatePass4 =
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

	ALPHA					// writeMask
};

static PipeStateDesc pass1 =
{
	lightIndexingPassesVS,
	lightIndexingPassesPS,
	0,
	0,
	backFaceRasterizerStateDesc,
	&depthStencilStateDesc,
	&outputMergerStatePass1
};

static PipeStateDesc pass2 =
{
	lightIndexingPassesVS,
	lightIndexingPassesPS,
	0,
	0,
	backFaceRasterizerStateDesc,
	&depthStencilStateDesc,
	&outputMergerStatePass2
};

static PipeStateDesc pass3 =
{
	lightIndexingPassesVS,
	lightIndexingPassesPS,
	0,
	0,
	backFaceRasterizerStateDesc,
	&depthStencilStateDesc,
	&outputMergerStatePass3
};

static PipeStateDesc pass4 =
{
	lightIndexingPassesVS,
	lightIndexingPassesPS,
	0,
	0,
	backFaceRasterizerStateDesc,
	&depthStencilStateDesc,
	&outputMergerStatePass4
};

PipeStateDesc* lightIndexingPass1PipeStateDesc = &pass1;
PipeStateDesc* lightIndexingPass2PipeStateDesc = &pass2;
PipeStateDesc* lightIndexingPass3PipeStateDesc = &pass3;
PipeStateDesc* lightIndexingPass4PipeStateDesc = &pass4;
