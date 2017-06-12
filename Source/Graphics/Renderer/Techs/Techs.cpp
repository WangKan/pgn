#include "LightIndexingTechDesc.h"
#include "NavMeshTechDesc.h"
#include "PostProcessingTechDesc.h"
#include "SkinnedMeshTechDesc.h"
#include "SkyBoxTechDesc.h"
#include "StaticMeshTechDesc.h"
#include "Techs.h"

TechDesc* techDescs[numTechs] =
{
	  lightIndexingTechDesc
	, staticMeshTechDesc
	, skinnedMeshTechDesc
	, skyBoxTechDesc
	, navMeshTechDesc
	, postProcessingTechDesc
};
