#include "EnvConstEnum.h"
#include "EnvDesc.h"
#include "RTs/RTs.h"

static unsigned constEnums[] =
{
	VIEW_PROJ
	, CAM_POS
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
