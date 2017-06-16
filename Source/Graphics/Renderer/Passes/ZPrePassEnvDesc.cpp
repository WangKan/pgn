#include "EnvConstEnum.h"
#include "EnvDesc.h"
#include "RTs/RTs.h"

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
	VIEW_PROJ
};

static EnvDesc desc =
{
	0,
	0,
	&depthStencilBuf,
	constEnums,
	sizeof(constEnums) / sizeof(EnvConstEnum)
};

EnvDesc* zPrePassEnvDesc = &desc;
