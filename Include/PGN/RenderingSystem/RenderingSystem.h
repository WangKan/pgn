#pragma once
#include "../Common/DllInterface.h"
#include "../Common/Interface.h"
namespace pgn {

class	Blob;
class	Buffer;
struct	BufferDesc;
class	DepthStencilState;
struct	DepthStencilStateDesc;
class	DepthStencilView;
class	OutputMergerState;
struct	OutputMergerStateDesc;
class	Program;
class	RasterizerState;
struct	RasterizerStateDesc;
class	RenderBuffer;
struct	RenderBufferDesc;
class	RenderingContext;
class	RenderTargetView;
class	Sampler;
struct	SamplerDesc;
class	Shader;
struct	ShaderDesc;
class	SyncPoint;
class	Texture;
struct	TextureDesc;
struct	VertexAttribDesc;
class	VertexInputLayout;

enum PrimType
{
	POINT_LIST,
	LINE_LIST,
	reserved_for_LINE_STRIP,
	TRIANGLE_LIST,
	TRIANGLE_STRIP
};

const int maxNumRenderTargets = 4;
const int maxNumTextures = 8;

class RenderingSystem : public Interface
{
public:
	static PGN_DLL_INTERFACE RenderingSystem* create(RenderingContext* rc);

	virtual void beginOffscreenPass(int numRenderTargets, RenderTargetView* renderTargets[], DepthStencilView* depthStencilView) = 0;
	virtual void beginOnscreenPass() = 0;
	virtual bool checkSyncPoint(SyncPoint* sync) = 0;
	virtual Buffer*				createBuffer(BufferDesc* desc) = 0;
	virtual DepthStencilState*	createDepthStencilState(DepthStencilStateDesc* desc) = 0;
	virtual DepthStencilView*	createDepthStencilView(RenderBuffer* rb) = 0;
	virtual DepthStencilView*	createDepthStencilView(Texture* tex) = 0;
	virtual OutputMergerState*	createOutputMergerState(OutputMergerStateDesc* desc) = 0;
	virtual Program*			createProgram(Shader* vs, Shader* ps) = 0;
	virtual Program*			createProgram(Blob* programBlob) = 0;
	virtual RasterizerState*	createRasterizerState(RasterizerStateDesc* desc) = 0;
	virtual RenderBuffer*		createRenderBuffer(RenderBufferDesc* desc) = 0;
	virtual RenderTargetView*	createRenderTargetView(RenderBuffer* rb) = 0;
	virtual RenderTargetView*	createRenderTargetView(Texture* tex) = 0;
	virtual Sampler*			createSampler(SamplerDesc* desc) = 0;
	virtual Shader*				createShader(ShaderDesc* desc) = 0;
	virtual SyncPoint*			createSyncPoint() = 0;
	virtual Texture*			createTexture(TextureDesc* desc, void* levels[] = 0) = 0;
	virtual VertexInputLayout*	createVertexInputLayout(Program* program, int numAttribs, const VertexAttribDesc attribDescs[], const int strides[]) = 0;
	virtual void clearDepthStencilView(DepthStencilView* depthStencilView/*0表示帧缓冲*/, bool clearDepth, float depth, bool clearStencil, unsigned char stencil) = 0;
	virtual void clearRenderTargetView(RenderTargetView* renderTargetView/*0表示帧缓冲*/, float r, float g, float b, float a) = 0;
	virtual void draw(int indicesPerInstance, int instanceCount) = 0;
	virtual void endPass(SyncPoint* sync) = 0;
	virtual void finish() = 0;
	virtual void flush() = 0;
	virtual int getConstantBufferOffsetAlignment() = 0;
	virtual Blob* serialize(Program* program) = 0;
	virtual void setConstantBuffers(int startSlot, int numBuffers, Buffer* buffers[], int offsets[], int sizes[]) = 0;
	virtual void setDepthStencilState(DepthStencilState* state, unsigned stencilRef) = 0;
	virtual void setIndexBuffer(Buffer* buf, int offset) = 0;
	virtual void setOutputMergerState(OutputMergerState* state/*, float blendFactor[4] = 0, float sampleCoverage = 1.0f*/) = 0;
	virtual void setPrimitiveType(PrimType primType) = 0;
	virtual void setProgram(Program* program/*0解绑*/) = 0;
	virtual void setRasterizerState(RasterizerState* state) = 0;
	virtual void setSamplers(int startSlot, int numSamplers, Sampler* samplers[]) = 0;
	virtual void setTextures(int startSlot, int numTextures, Texture* textures[]) = 0;
	virtual void setVertexBuffers(int startSlot, int numBuffers, Buffer* buffers[], int offsets[]) = 0;
	virtual void setVertexInputLayout(VertexInputLayout* layout) = 0;
	virtual void setViewport(int left, int top, int width, int height, int fullHeight, float minDepth, float maxDepth) = 0;
};

}
