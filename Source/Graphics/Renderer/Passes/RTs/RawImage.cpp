#include <PGN/RenderingSystem/Texture.h>
#include "ResourceViewDesc.h"

static pgn::TextureDesc rawImageDesc =
{
	pgn::RGBA8,
	1,
	1,
	1
};

static ResourceViewDesc _rawImageRTViewDesc =
{
	RENDER_TARGET_VIEW,
	"rawImage",
	"rawImage",
	&rawImageDesc,
	1.0f
};

ResourceViewDesc* rawImageRTViewDesc = &_rawImageRTViewDesc;
