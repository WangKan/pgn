
struct RenderTargetDesc
{
	unsigned resViewEnum;
	bool clearNeeded;
	float clearValue[4];
};

struct DepthStencilBufDesc
{
	unsigned resViewEnum;

	bool depthClearNeeded;
	float depthClearValue;

	bool stencilClearNeeded;
	unsigned char stencilClearValue;
};

struct EnvDesc
{
	RenderTargetDesc*		offscreenRTDescs;
	int						numOffscreenRTs;
	DepthStencilBufDesc*	offscreenDepthStencilBufDesc;
	unsigned*               constEnums;
	int						numConsts;
};
