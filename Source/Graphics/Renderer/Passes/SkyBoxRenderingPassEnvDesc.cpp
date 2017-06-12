#include "EnvConstEnum.h"
#include "EnvDesc.h"
#include "RTs/RTs.h"

static RenderTargetDesc rts[] =
{
	RAW_IMAGE_RT_VIEW,
	false,
	0.0f, 0.0f, 0.0f, 0.0f,
};

static DepthStencilBufDesc depthStencilBuf =
{
	DEPTH_STENCIL_MAP_DS_VIEW,

	false,
	0,

	false,
	0
};

static unsigned constEnums[] =
{
	VIEW_PROJ
};

static EnvDesc desc =
{
	rts,
	sizeof(rts) / sizeof(rts[0]),
	&depthStencilBuf,
	constEnums,
	sizeof(constEnums) / sizeof(EnvConstEnum)
};

EnvDesc* skyBoxRenderingPassEnvDesc = &desc;
