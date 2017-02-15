
namespace pgn {
	struct  TextureDesc;
}

enum ResourceViewType
{
	RENDER_TARGET_VIEW,
	DEPTH_STENCIL_VIEW
};

struct ResourceViewDesc
{
	ResourceViewType type;
	char* bindingPointName;
	char* resName;
	pgn::TextureDesc* texDesc;
	float autoSizeScale;
};
