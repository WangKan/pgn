#include <PGN/RenderingSystem/DepthStencilState.h>
#include <PGN/RenderingSystem/OutputMergerState.h>
#include <PGN/RenderingSystem/RasterizerState.h>
#include <PGN/RenderingSystem/Sampler.h>

using namespace pgn;

static SamplerDesc _diffuseMapSamplerDesc =
{
	MIN_LINEAR_MIP_LINEAR,	// MinFilter
	MAG_LINEAR,				// MagFilter
	CLAMP,					// wrapU
	CLAMP,					// wrapV
	16,						// maxAniso
	ALWAYS					// cmpFunc
};
SamplerDesc* diffuseMapSamplerDesc = &_diffuseMapSamplerDesc;


static SamplerDesc _pointSamplerDesc =
{
	MIN_POINT,				// MinFilter
	MAG_POINT,				// MagFilter
	CLAMP,					// wrapU
	CLAMP,					// wrapV
	1,						// maxAniso
	ALWAYS					// cmpFunc
};
SamplerDesc* pointSamplerDesc = &_pointSamplerDesc;


static RasterizerStateDesc _commonRasterizerStateDesc =
{
	CCW,					// FrontFace
	BACK_FACE				// CullFace
};
RasterizerStateDesc* commonRasterizerStateDesc = &_commonRasterizerStateDesc;


static RasterizerStateDesc _backFaceRasterizerStateDesc =
{
	CCW,					// FrontFace
	FRONT_FACE				// CullFace
};
RasterizerStateDesc* backFaceRasterizerStateDesc = &_backFaceRasterizerStateDesc;


static RasterizerStateDesc _twoSidedRasterizerStateDesc =
{
	CCW,					// FrontFace
	NONE					// CullFace
};
RasterizerStateDesc* twoSidedRasterizerStateDesc = &_twoSidedRasterizerStateDesc;


static DepthStencilStateDesc _commonDepthStencilStateDesc =
{
	true,					// depthEnable
	true,					// depthWrite
	LESS,					// depthFunc

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
DepthStencilStateDesc* commonDepthStencilStateDesc = &_commonDepthStencilStateDesc;


static DepthStencilStateDesc _depthStencilTestingDisabled =
{
	false,					// depthEnable
	false,					// depthWrite
	ALWAYS,					// depthFunc

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
DepthStencilStateDesc* depthStencilTestingDisabledStateDesc = &_depthStencilTestingDisabled;


static OutputMergerStateDesc _blendingDisabled =
{
	//false,					// alphaToCoverageEnable

	//renderTargets[0]
	false,					// blendEnable
	ADD,					// opRGB
	SRC_ALPHA,				// factorSrcRGB
	INV_SRC_ALPHA,			// factorDestRGB
	ADD,					// opAlpha
	ZERO,					// factorSrcAlpha
	ONE,					// factorDestAlpha

	RED|GREEN|BLUE|ALPHA	// writeMask
};
OutputMergerStateDesc* blendingDisabledStateDesc = &_blendingDisabled;


static OutputMergerStateDesc _alphaBlending =
{
	//false,					// alphaToCoverageEnable

	//renderTargets[0]
	true,					// blendEnable
	ADD,					// opRGB
	SRC_ALPHA,				// factorSrcRGB
	INV_SRC_ALPHA,			// factorDestRGB
	ADD,					// opAlpha
	ZERO,					// factorSrcAlpha
	ONE,					// factorDestAlpha

	RED|GREEN|BLUE|ALPHA	// writeMask
};
OutputMergerStateDesc* alphaBlendingStateDesc = &_alphaBlending;
