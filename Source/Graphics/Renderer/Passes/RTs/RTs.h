
struct ResourceViewDesc;

enum ResourceViewEnum
{
	DEPTH_STENCIL_MAP_DS_VIEW,
	G_BUF_0_RT_VIEW,
	G_BUF_1_RT_VIEW,
	LIGHT_INDEX_MAP_RT_VIEW,

	numResViews
};

extern ResourceViewDesc* resViewDescs[numResViews];
