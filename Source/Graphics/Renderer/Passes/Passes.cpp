#include "ForwardShadingPassEnvDesc.h"
#include "GeometryPassEnvDesc.h"
#include "LightIndexingPassesEnvDescs.h"
#include "LightingPassEnvDesc.h"
#include "LightVolumePassesEnvDescs.h"
#include "Passes.h"
#include "TransparencyZPrePassEnvDesc.h"
#include "ZPrePassEnvDesc.h"

EnvDesc* envDescs[numPasses] =
{
	zPrePassEnvDesc,
	geometryPassEnvDesc,
	lightVolumePass1EnvDesc,
	lightVolumePass2EnvDesc,
	lightIndexingPass1PipeStateEnvDesc,
	lightVolumePass1EnvDesc,
	lightVolumePass2EnvDesc,
	lightIndexingPass2PipeStateEnvDesc,
	lightVolumePass1EnvDesc,
	lightVolumePass2EnvDesc,
	lightIndexingPass3PipeStateEnvDesc,
	lightVolumePass1EnvDesc,
	lightVolumePass2EnvDesc,
	lightIndexingPass4PipeStateEnvDesc,
	lightingPassEnvDesc,
	forwardShadingPassEnvDesc,
	transparencyZPrePassEnvDesc,
	transparencyForwardShadingPassEnvDesc,
};
