#include "LightIndexingTechDesc.h"
#include "NavMeshTechDesc.h"
#include "PostProcessingTechDesc.h"
#include "SkinnedMeshTechDesc.h"
#include "StaticMeshTechDesc.h"
#include "techs.h"

TechDesc* techDescs[numTechs] =
{
	  lightIndexingTechDesc
	, staticMeshTechDesc
	, skinnedMeshTechDesc
	, navMeshTechDesc
	, postProcessingTechDesc
};
