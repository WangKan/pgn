#include "EnvConstEnum.h"
#include "EnvDesc.h"

static unsigned constEnums[] =
{
	  INV_PROJ
	, V_POINT_LIGHT
	, V_DIR_LIGHT
};

static EnvDesc desc =
{
	0,
	0,
	0,
	constEnums,
	sizeof(constEnums) / sizeof(EnvConstEnum)
};

EnvDesc* lightingPassEnvDesc = &desc;
