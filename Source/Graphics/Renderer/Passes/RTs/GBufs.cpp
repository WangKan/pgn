#include <PGN/RenderingSystem/Texture.h>
#include "ResourceViewDesc.h"

static pgn::TextureDesc gbuf0Desc =
{
	pgn::RGBA8,
	1,
	1,
	1
};

static ResourceViewDesc _gbuf0RTViewDesc =
{
	RENDER_TARGET_VIEW,
	"gbuf0",
	"gbuf0",
	&gbuf0Desc,
	1.0f
};

ResourceViewDesc* gbuf0RTViewDesc = &_gbuf0RTViewDesc;


static pgn::TextureDesc gbuf1Desc =
{
	pgn::RGBA8,
	1,
	1,
	1
};

static ResourceViewDesc _gbuf1RTViewDesc =
{
	RENDER_TARGET_VIEW,
	"gbuf1",
	"gbuf1",
	&gbuf1Desc,
	1.0f
};

ResourceViewDesc* gbuf1RTViewDesc = &_gbuf1RTViewDesc;
