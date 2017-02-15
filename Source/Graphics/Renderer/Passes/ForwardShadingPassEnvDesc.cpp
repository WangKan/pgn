#include "EnvConstEnum.h"
#include "EnvDesc.h"
#include "RTs/RTs.h"

static unsigned constEnums[] =
{
	  CAM_POS
	, VIEW_PROJ
	, W_POINT_LIGHT
	, W_DIR_LIGHT
};

static EnvDesc desc =
{
	0,
	0,
	0,
	constEnums,
	sizeof(constEnums) / sizeof(EnvConstEnum)
};

EnvDesc* forwardShadingPassEnvDesc = &desc;
EnvDesc* transparencyForwardShadingPassEnvDesc = &desc;
