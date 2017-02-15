#include <PGN/RenderingSystem/Texture.h>
#include "ResourceViewDesc.h"

static pgn::TextureDesc lightIndexMapDesc =
{
	pgn::RGBA8UI,
	1,
	1,
	1
};

static ResourceViewDesc _lightIndexMapRTViewDesc =
{
	RENDER_TARGET_VIEW,
	"lightIndexMap",
	"lightIndexMap",
	&lightIndexMapDesc,
	1.0f
};

ResourceViewDesc* lightIndexMapRTViewDesc = &_lightIndexMapRTViewDesc;
