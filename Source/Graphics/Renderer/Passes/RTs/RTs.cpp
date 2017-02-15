#include "DepthStencilMap.h"
#include "GBufs.h"
#include "LightIndexMap.h"
#include "RTs.h"

ResourceViewDesc* resViewDescs[numResViews] =
{
	  depthStencilMapDSViewDesc
	, gbuf0RTViewDesc
	, gbuf1RTViewDesc
	, lightIndexMapRTViewDesc
};
