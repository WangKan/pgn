#include <PGN/RenderingSystem/Texture.h>
#include "ResourceViewDesc.h"

static pgn::TextureDesc depthStencilMapDesc =
{
	pgn::D24_S8,
	1,
	1,
	1
};

static ResourceViewDesc _depthStencilMapDSViewDesc =
{
	DEPTH_STENCIL_VIEW,
	"depthStencilMap",
	"depthStencilMap",
	&depthStencilMapDesc,
	1.0f
};

ResourceViewDesc* depthStencilMapDSViewDesc = &_depthStencilMapDSViewDesc;
