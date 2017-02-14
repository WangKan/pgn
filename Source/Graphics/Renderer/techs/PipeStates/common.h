namespace pgn {
	struct DepthStencilStateDesc;
	struct OutputMergerStateDesc;
	struct RasterizerStateDesc;
	struct SamplerDesc;
}

extern pgn::SamplerDesc* diffuseMapSamplerDesc;
extern pgn::SamplerDesc* pointSamplerDesc;
extern pgn::RasterizerStateDesc* commonRasterizerStateDesc;
extern pgn::RasterizerStateDesc* backFaceRasterizerStateDesc;
extern pgn::RasterizerStateDesc* twoSidedRasterizerStateDesc;
extern pgn::DepthStencilStateDesc* commonDepthStencilStateDesc;
extern pgn::DepthStencilStateDesc* depthStencilTestingDisabledStateDesc;
extern pgn::OutputMergerStateDesc* blendingDisabledStateDesc;
extern pgn::OutputMergerStateDesc* alphaBlendingStateDesc;
