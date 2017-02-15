#include "EnvConstEnum.h"
#include "EnvDesc.h"
#include "RTs/RTs.h"

static RenderTargetDesc rts[] =
{
	G_BUF_0_RT_VIEW,
	true,
	0.0f, 0.0f, 0.0f, 0.0f,

	G_BUF_1_RT_VIEW,
	true,
	0.0f, 0.0f, 0.0f, 0.0f
};

static DepthStencilBufDesc depthStencilBuf =
{
	DEPTH_STENCIL_MAP_DS_VIEW,

	true,
	1.0f,

	false,
	0
};

static unsigned constEnums[] =
{
	  CAM_POS
	, VIEW
	, VIEW_PROJ
};

static EnvDesc desc =
{
	rts,
	sizeof(rts) / sizeof(rts[0]),
	&depthStencilBuf,
	constEnums,
	sizeof(constEnums) / sizeof(EnvConstEnum)
};

EnvDesc* geometryPassEnvDesc = &desc;
