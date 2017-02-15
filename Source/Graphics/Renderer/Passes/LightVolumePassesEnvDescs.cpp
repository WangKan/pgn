#include "EnvConstEnum.h"
#include "EnvDesc.h"
#include "RTs/RTs.h"

static DepthStencilBufDesc depthStencilBufPass1 =
{
	DEPTH_STENCIL_MAP_DS_VIEW,

	false,
	0,

	true,
	0
};

static DepthStencilBufDesc depthStencilBufPass2 =
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

static EnvDesc pass1 =
{
	0,
	0,
	&depthStencilBufPass1,
	constEnums,
	sizeof(constEnums) / sizeof(EnvConstEnum)
};

static EnvDesc pass2 =
{
	0,
	0,
	&depthStencilBufPass2,
	constEnums,
	sizeof(constEnums) / sizeof(EnvConstEnum)
};

EnvDesc* lightVolumePass1EnvDesc = &pass1;
EnvDesc* lightVolumePass2EnvDesc = &pass2;
