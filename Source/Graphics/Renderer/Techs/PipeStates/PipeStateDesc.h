
namespace pgn {
	struct DepthStencilStateDesc;
	struct OutputMergerStateDesc;
	struct RasterizerStateDesc;
	struct SamplerDesc;
}

struct PipeStateDesc
{
#ifdef usePrecompiledShader
	void*							vs;
	int								sizeVS;
	void*							ps;
	int								sizePS;
#else
	char*							vs;
	char*							ps;
#endif
	pgn::SamplerDesc**				samplerDescs;
	int								numSamplers;
	pgn::RasterizerStateDesc*		rasterizerStateDesc;
	pgn::DepthStencilStateDesc*		depthStencilStateDesc;
	pgn::OutputMergerStateDesc*		outputMergerStateDesc;
};
