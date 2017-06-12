#include "EnvConstEnum.h"
#include "EnvDesc.h"
#include "RTs/RTs.h"

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
	  CAM_POS
	, VIEW_PROJ
};

static EnvDesc desc =
{
	0,
	0,
	&depthStencilBuf,
	constEnums,
	sizeof(constEnums) / sizeof(EnvConstEnum)
};

EnvDesc* transparencyZPrePassEnvDesc = &desc;
