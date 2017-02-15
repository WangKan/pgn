#include "EnvConstEnum.h"
#include "EnvDesc.h"
#include "RTs/RTs.h"

static RenderTargetDesc rtsPass1[] =
{
	LIGHT_INDEX_MAP_RT_VIEW,
	true,
	1.0f, 1.0f, 1.0f, 1.0f
};

static RenderTargetDesc rtsPass234[] =
{
	LIGHT_INDEX_MAP_RT_VIEW,
	false,
	0.0f, 0.0f, 0.0f, 0.0f
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

static EnvDesc pass1 =
{
	rtsPass1,
	sizeof(rtsPass1) / sizeof(rtsPass1[0]),
	&depthStencilBuf,
	constEnums,
	sizeof(constEnums) / sizeof(EnvConstEnum)
};

static EnvDesc pass234 =
{
	rtsPass234,
	sizeof(rtsPass234) / sizeof(rtsPass234[0]),
	&depthStencilBuf,
	constEnums,
	sizeof(constEnums) / sizeof(EnvConstEnum)
};

EnvDesc* lightIndexingPass1PipeStateEnvDesc = &pass1;
EnvDesc* lightIndexingPass2PipeStateEnvDesc = &pass234;
EnvDesc* lightIndexingPass3PipeStateEnvDesc = &pass234;
EnvDesc* lightIndexingPass4PipeStateEnvDesc = &pass234;
