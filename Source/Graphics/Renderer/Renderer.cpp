/*

	晚上教授回家以后，学生们就聚在一起学习，游戏，摸索，钻研。计算机对他们来说是前所未有的工具。
	在这个天地里，一个人只要努力就肯定可以做到他想做的事情。这些程序员不去上课，取消约会，甚至
	连澡都不洗，就在这个天地里遨游。一旦他们有了足够的知识，他们要编写的程序就是————游戏。

																	———— 《DOOM启世录》

*/

#include <math.h>
#include <PGN/Common/Blob.h>
#include <PGN/Common/debug_new.h>
#include <PGN/FileStream/FileStream.h>
#include <PGN/Platform/pageAlloc.h>
#include <PGN/Platform/UI/RenderingContext.h>
#include <PGN/Platform/UI/Window.h>
#include <PGN/RenderingSystem/DepthStencilState.h>
#include <PGN/RenderingSystem/DepthStencilView.h>
#include <PGN/RenderingSystem/OutputMergerState.h>
#include <PGN/RenderingSystem/Program.h>
#include <PGN/RenderingSystem/RasterizerState.h>
#include <PGN/RenderingSystem/RenderTargetView.h>
#include <PGN/RenderingSystem/Sampler.h>
#include <PGN/RenderingSystem/Shader.h>
#include <PGN/RenderingSystem/SyncPoint.h>
#include <PGN/RenderingSystem/Texture.h>
#include <PGN/Utilities/LinearTransformations.h>
#include <PGN/Utilities/Pipeline.h>
#include <PGN/Utilities/ResourceManager/AsyncLoader.h>
#include <PGN/Utilities/ResourceManager/ResourceHandle.h>
#include <PGN/Utilities/ResourceManager/ResourceManager.h>
#include "Assets/EditablePNMFactory.h"
#include "Assets/NAVFactory.h"
#include "Assets/PNMFactory.h"
#include "Assets/PNTFactory.h"
#include "CBufAllocator.h"
#include "GeometryHelper.h"
#include "Passes/EnvDesc.h"
#include "Renderer.h"
#include "Techs/PipeStates/PipeStateDesc.h"
#include "Techs/TechDesc.h"
#include "TextureInfo.h"

struct ResourceView
{
	ResourceViewType type;
	char* bindingPointName;
	char* resName;
	pgn::Texture* tex;
	pgn::ResourceHandle* texHandle;
	pgn::ResourceView* view;
	float autoSizeScale;
};

PassEnum transparentEntityPasses[] =
{
	TRANSPARENCY_Z_PRE_PASS,
	TRANSPARENCY_SHADING_PASS
};

PassEnum lightIndexedDeferredRenderingActivePasses[] =
{
	GEOMETRY_PASS,
	LIGHT_VOLUME_BACK_FACE_PASS_1,
	LIGHT_VOLUME_FRONT_FACE_PASS_1,
	LIGHT_INDEXING_PASS_1,
	LIGHT_VOLUME_BACK_FACE_PASS_2,
	LIGHT_VOLUME_FRONT_FACE_PASS_2,
	LIGHT_INDEXING_PASS_2,
	LIGHT_VOLUME_BACK_FACE_PASS_3,
	LIGHT_VOLUME_FRONT_FACE_PASS_3,
	LIGHT_INDEXING_PASS_3,
	LIGHT_VOLUME_BACK_FACE_PASS_4,
	LIGHT_VOLUME_FRONT_FACE_PASS_4,
	LIGHT_INDEXING_PASS_4,
	LIGHTING_PASS,
	FORWARD_SHADING_PASS,
	TRANSPARENCY_Z_PRE_PASS,
	TRANSPARENCY_SHADING_PASS,
};

PassEnum lightIndexedDeferredRenderingOpaqueEntityPasses[] =
{
	GEOMETRY_PASS
};

PassEnum lightIndexedDeferredRenderingPostProcessingPasses[] =
{
	LIGHTING_PASS
};

RendererConfig lightIndexedDeferredRendering =
{
	lightIndexedDeferredRenderingActivePasses,
	sizeof(lightIndexedDeferredRenderingActivePasses) / sizeof(lightIndexedDeferredRenderingActivePasses[0]),

	lightIndexedDeferredRenderingOpaqueEntityPasses,
	sizeof(lightIndexedDeferredRenderingOpaqueEntityPasses) / sizeof(lightIndexedDeferredRenderingOpaqueEntityPasses[0]),

	transparentEntityPasses,
	sizeof(transparentEntityPasses) / sizeof(transparentEntityPasses[0]),

	lightIndexedDeferredRenderingPostProcessingPasses,
	sizeof(lightIndexedDeferredRenderingPostProcessingPasses) / sizeof(lightIndexedDeferredRenderingPostProcessingPasses[0])
};

PassEnum lightIndexedForwardRenderingActivePasses[] =
{
	Z_PRE_PASS,
	LIGHT_VOLUME_BACK_FACE_PASS_1,
	LIGHT_VOLUME_FRONT_FACE_PASS_1,
	LIGHT_INDEXING_PASS_1,
	LIGHT_VOLUME_BACK_FACE_PASS_2,
	LIGHT_VOLUME_FRONT_FACE_PASS_2,
	LIGHT_INDEXING_PASS_2,
	LIGHT_VOLUME_BACK_FACE_PASS_3,
	LIGHT_VOLUME_FRONT_FACE_PASS_3,
	LIGHT_INDEXING_PASS_3,
	LIGHT_VOLUME_BACK_FACE_PASS_4,
	LIGHT_VOLUME_FRONT_FACE_PASS_4,
	LIGHT_INDEXING_PASS_4,
	FORWARD_SHADING_PASS,
	TRANSPARENCY_Z_PRE_PASS,
	TRANSPARENCY_SHADING_PASS,
};

PassEnum lightIndexedForwardRenderingOpaqueEntityPasses[] =
{
	Z_PRE_PASS,
	FORWARD_SHADING_PASS
};

RendererConfig lightIndexedForwardRendering =
{
	lightIndexedForwardRenderingActivePasses,
	sizeof(lightIndexedForwardRenderingActivePasses) / sizeof(lightIndexedForwardRenderingActivePasses[0]),

	lightIndexedForwardRenderingOpaqueEntityPasses,
	sizeof(lightIndexedForwardRenderingOpaqueEntityPasses) / sizeof(lightIndexedForwardRenderingOpaqueEntityPasses[0]),

	transparentEntityPasses,
	sizeof(transparentEntityPasses) / sizeof(transparentEntityPasses[0]),

	0,
	0
};

FrameContext::FrameContext(pgn::RenderingSystem* rs)
{
	heap = pgn::Heap::create();
	cbufAllocator = debug_new CBufAllocator(rs);
	sync = rs->createSyncPoint();
}

FrameContext::~FrameContext()
{
	for (auto& passBatches : batches)
	{
		for (auto& techBatches : passBatches)
		{
			BatchGroupMap& batchGroupMap = techBatches.second;
			for (auto& entry : batchGroupMap)
			{
				BatchGroup* batchGroup = entry.second.p;
				delete batchGroup;
			}
		}
	}
	heap->destroy();
	delete cbufAllocator;
	sync->destroy();
}

class RenderingStage : public pgn::PipelineStage
{
	Renderer* renderer;
	pgn::Window* wnd;

public:
	RenderingStage(Renderer* renderer, pgn::Window* wnd)
	{
		this->renderer = renderer;
		this->wnd = wnd;
	}

	virtual void onBegin()
	{
		renderer->rc->beginDraw(wnd->getDisplay(), 1);
	}

	virtual void onEnd()
	{
		renderer->rc->endDraw();
	}

	virtual void process(void* p)
	{
		FrameContext* frameContext = *((FrameContext**)p);

		renderer->render(frameContext);
		wnd->present();
	}
};

Renderer::Renderer(pgn::Display displayPrototype, pgn::FileStream* assetStream, pgn::FileStream* cacheStream)
	: retired(maxNumPrerenderedFrames)
{
	getVertexFormatTable();

	rc = pgn::RenderingContext::create(displayPrototype, 3);

	pnmFactory = debug_new PNMFactory(subsetAllocator);
	editablePnmFactory = debug_new EditablePNMFactory(subsetAllocator);
	navFactory = debug_new NAVFactory(subsetAllocator);
	pntFactory = debug_new PNTFactory;

	heap = pgn::Heap::create();

	this->assetStream = assetStream;
	this->cacheStream = cacheStream;

	envConsts[CAM_POS].size = sizeof(pgn::Float3);
	envConsts[VIEW].size = sizeof(pgn::Float4x3);
	envConsts[VIEW_PROJ].size = sizeof(pgn::Float4x4);
	envConsts[INV_PROJ].size = sizeof(pgn::Float4x4);
	envConsts[W_POINT_LIGHT].size = sizeof(((FrameContext*)0)->wPointLights);
	envConsts[V_POINT_LIGHT].size = sizeof(((FrameContext*)0)->vPointLights);
	envConsts[W_DIR_LIGHT].size = sizeof(((FrameContext*)0)->wDirLights);
	envConsts[V_DIR_LIGHT].size = sizeof(((FrameContext*)0)->vDirLights);
}

Renderer::~Renderer()
{
	releaseVertexFormatTable();

	rc->destroy();

	delete pnmFactory;
	delete editablePnmFactory;
	delete navFactory;
	delete pntFactory;

	heap->destroy();
}

#ifdef usePrecompiledShader
bool Renderer::buildPrograms(void* cacheFileBuf)
{
	for (int i = 0; i < numTechs; i++)
	{
		TechDesc* techDesc = techDescs[i];
		Tech* tech = &techs[i];

		for (int j = 0; j < numPasses; j++)
		{
			PipeStateDesc* pipeStateDesc = techDesc->pipeStateDescs[j];

			if (!pipeStateDesc)
			{
				tech->pipeSet[j] = 0;
				continue;
			}

			PipeState* pipe = debug_new PipeState;
			tech->pipeSet[j] = pipe;

			pgn::ShaderDesc vsDesc = { pgn::VERTEX_SHADER, pipeStateDesc->vs, pipeStateDesc->sizeVS };
			pipe->vs = rs->createShader(&vsDesc);

			pgn::ShaderDesc psDesc = { pgn::PIXEL_SHADER, pipeStateDesc->ps, pipeStateDesc->sizePS };
			pipe->ps = rs->createShader(&psDesc);

			pgn::Program* program = rs->createProgram(pipe->vs, pipe->ps);
			pipe->program = program;

			int numTextureVariables = program->getNumTextureVariables();
			assert(pipeStateDesc->numSamplers == numTextureVariables);

			pipe->texBindingPoints.resize(numTextureVariables);

			for (int k = 0; k < numTextureVariables; k++)
			{
				pipe->texBindingPoints[k] = &texBindingPointMap[program->getTextureVariableName(k)];
			}

			for (int k = 0; k < pipeStateDesc->numSamplers; k++)
			{
				pgn::SamplerDesc* samplerDesc = pipeStateDesc->samplerDescs[k];
				pgn::Sampler* sampler = rs->createSampler(samplerDesc);
				pipe->samplers.push_back(sampler);
			}

			pipe->rasterizerState = rs->createRasterizerState(pipeStateDesc->rasterizerStateDesc);
			pipe->depthStencilState = rs->createDepthStencilState(pipeStateDesc->depthStencilStateDesc);
			pipe->outputMergerState = rs->createOutputMergerState(pipeStateDesc->outputMergerStateDesc);
		}
	}

	return true;
}
#else
bool Renderer::buildPrograms(void* cacheFileBuf)
{
	struct ProgramBlobHead
	{
		int size;
		char data[1];
	};

	ProgramBlobHead* programBlobHead = (ProgramBlobHead*)cacheFileBuf;

	for (int i = 0; i < numTechs; i++)
	{
		TechDesc* techDesc = techDescs[i];
		Tech* tech = &techs[i];

		for (int j = 0; j < numPasses; j++)
		{
			PipeStateDesc* pipeStateDesc = techDesc->pipeStateDescs[j];

			if (!pipeStateDesc)
			{
				tech->pipeSet[j] = 0;
				continue;
			}

			PipeState* pipe = debug_new PipeState;
			tech->pipeSet[j] = pipe;

			pgn::Program* program;

			if (programBlobHead)
			{
				pipe->vs = 0;
				pipe->ps = 0;

				pgn::Blob* programBlob = pgn::Blob::create(programBlobHead->size);
				memcpy(programBlob->getBuffer(), programBlobHead->data, programBlobHead->size);

				program = rs->createProgram(programBlob);

				programBlob->destroy();
				programBlobHead = (ProgramBlobHead*)(programBlobHead->data + programBlobHead->size);

				if (!program)
				{
					delete pipe;
					return false;
				}
			}
			else
			{
				pgn::ShaderDesc vsDesc = { pgn::VERTEX_SHADER, pipeStateDesc->vs };
				pipe->vs = rs->createShader(&vsDesc);

				pgn::ShaderDesc psDesc = { pgn::PIXEL_SHADER, pipeStateDesc->ps };
				pipe->ps = rs->createShader(&psDesc);

				program = rs->createProgram(pipe->vs, pipe->ps);

				if (cacheStream)
				{
					pgn::Blob* programBlob = rs->serialize(program);

					int programBlobSize = programBlob->getSize();
					cacheStream->write(&programBlobSize, sizeof(programBlobSize));
					cacheStream->write(programBlob->getBuffer(), programBlobSize);

					programBlob->destroy();
				}
			}

			pipe->program = program;

			int numTextureVariables = program->getNumTextureVariables();
			assert(pipeStateDesc->numSamplers == numTextureVariables);

			pipe->texBindingPoints.resize(numTextureVariables);

			for (int k = 0; k < numTextureVariables; k++)
			{
				pipe->texBindingPoints[k] = &texBindingPointMap[program->getTextureVariableName(k)];
			}

			for (int k = 0; k < pipeStateDesc->numSamplers; k++)
			{
				pgn::SamplerDesc* samplerDesc = pipeStateDesc->samplerDescs[k];
				pgn::Sampler* sampler = rs->createSampler(samplerDesc);
				pipe->samplers.push_back(sampler);
			}

			pipe->rasterizerState = rs->createRasterizerState(pipeStateDesc->rasterizerStateDesc);
			pipe->depthStencilState = rs->createDepthStencilState(pipeStateDesc->depthStencilStateDesc);
			pipe->outputMergerState = rs->createOutputMergerState(pipeStateDesc->outputMergerStateDesc);
		}
	}

	return true;
}
#endif

void Renderer::beginDraw(pgn::Window* wnd, RendererConfig* _cfg)
{
	cfg = *_cfg;

	for (int i = 0; i < numPasses; i++)
		isPassActive[i] = false;

	for (int i = 0; i < cfg.numActivePasses; i++)
		isPassActive[cfg.activePasses[i]] = true;

	pgn::Display display = wnd->getDisplay();

	rc->beginDraw(display, 2);
	rs = pgn::RenderingSystem::create(rc);

	resLoader = pgn::createAsyncLoader(rc, rs, display);
	geomMgr = pgn::ResourceManager::create(pnmFactory, assetStream, resLoader);
	editableGeomMgr = pgn::ResourceManager::create(editablePnmFactory, assetStream, resLoader);
	navGeomMgr = pgn::ResourceManager::create(navFactory, assetStream, resLoader);
	texMgr = pgn::ResourceManager::create(pntFactory, assetStream, resLoader);

	// 创建灰色纹理
	{
		unsigned char level0[][4] =
		{
			127, 127, 127, 255
		};

		void* levels[] =
		{
			level0
		};

		pgn::TextureDesc texDesc;
		texDesc.format = pgn::RGBA8;
		texDesc.width = 1;
		texDesc.height = 1;
		texDesc.numLevels = sizeof(levels) / sizeof(levels[0]);

		pgn::Texture* tex = rs->createTexture(&texDesc, levels);
		gray = texMgr->addResource("gray", tex);
	}

	GeometryHelper helper(rs);

	// 创建screen-aligned quad
	{
		float pos[][2] =
		{
			-1.0f, -1.0f,
			1.0f, -1.0f,
			1.0f, 1.0f,
			-1.0f, 1.0f
		};

		float tc[][2] =
		{
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f
		};

		unsigned short indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		int numIndices[] =
		{
			sizeof(indices) / sizeof(short)
		};

		pgn::VertexAttribDesc vertAttribDescs[] =
		{
			"position2D", pgn::FLOAT2, 0,
			"texCoord0", pgn::FLOAT2, 0
		};

		void* attribs[] =
		{
			pos, tc
		};

		Geometry* geom = debug_new Geometry(subsetAllocator);

		helper.createGeometry(
			geom,
			sizeof(vertAttribDescs) / sizeof(pgn::VertexAttribDesc),
			vertAttribDescs,
			sizeof(pos) / sizeof(pos[0]),
			sizeof(numIndices) / sizeof(numIndices[0]),
			numIndices,
			attribs,
			indices
			);

		geom->primType = pgn::PrimType::TRIANGLE_LIST;
		geom->skeletonTemplate = 0;
		geom->brdfCoeffBuf.buf = 0;

		screenAlignedQuad = geomMgr->addResource("screen-aligned quad", geom);
	}

	// 创建球体
	{
		#define patchPoints(segs) (3 + ((segs) + 4) * ((segs) - 1) / 2)

		const int segs = 2;

		const int innerPoints = patchPoints(segs - 3);
		const int outerPoints = patchPoints(segs) - innerPoints;
		const int patchVerts = 3;
		const int edgePoints = outerPoints - patchVerts;
		const int spherePoints = patchVerts * 20 / 5 + edgePoints * 20 / 2 + innerPoints * 20;

		const int patchFaces = (1 + (1 + 2 * (segs - 1))) / 2 * segs;
		const int sphereFaces = patchFaces * 20;

		float pos[spherePoints][3];
		unsigned short indices[sphereFaces * 3];

		const double phi = 1.618033988749895;
		const double icosaVerts[12][3] =
		{
			1.0, phi, 0.0,

			-1.0, phi, 0.0,
			-phi, 0.0, -1.0,
			0.0, 1.0, -phi,
			0.0, -1.0, -phi,
			phi, 0.0, -1.0,
			1.0, -phi, 0.0,
			phi, 0.0, 1.0,
			0.0, -1.0, phi,
			0.0, 1.0, phi,
			-phi, 0.0, 1.0,

			-1.0, -phi, 0.0,
		};

		static int icosaFaces[20][3] =
		{
			0, 1, 3,
			0, 3, 5,
			0, 5, 7,
			0, 7, 9,
			0, 9, 1,

			1, 2, 3,
			4, 3, 2,
			3, 4, 5,
			6, 5, 4,
			5, 6, 7,
			8, 7, 6,
			7, 8, 9,
			10, 9, 8,
			9, 10, 1,
			2, 1, 10,

			11, 4, 2,
			11, 6, 4,
			11, 8, 6,
			11, 10, 8,
			11, 2, 10
		};

		int numVerts = 0;

		auto addPoint = [&](float x, float y, float z)
		{
			int i;

			for (i = 0; i < numVerts; i++)
			{
				float dx = x - pos[i][0];
				float dy = y - pos[i][1];
				float dz = z - pos[i][2];

				if (dx < 0.0f) dx = -dx;
				if (dy < 0.0f) dy = -dy;
				if (dz < 0.0f) dz = -dz;

				if (dx < 0.0001f && dy < 0.0001f && dz < 0.0001f) break;
			}

			if (i == numVerts)
			{
				pos[i][0] = x;
				pos[i][1] = y;
				pos[i][2] = z;

				numVerts++;
			}

			return i;
		};

		int _numIndices = 0;

		for (auto patch : icosaFaces)
		{
			int pointIndices[patchPoints(segs)];
			int numPoints = 0;

			for (int i = 0; i <= segs; i++)
			{
				double a[3], b[3];

				double t = double(i) / double(segs);
				double k0 = 1.0 - t;
				double k1 = t;

				a[0] = k0 * icosaVerts[patch[0]][0] + k1 * icosaVerts[patch[1]][0];
				a[1] = k0 * icosaVerts[patch[0]][1] + k1 * icosaVerts[patch[1]][1];
				a[2] = k0 * icosaVerts[patch[0]][2] + k1 * icosaVerts[patch[1]][2];

				b[0] = k0 * icosaVerts[patch[0]][0] + k1 * icosaVerts[patch[2]][0];
				b[1] = k0 * icosaVerts[patch[0]][1] + k1 * icosaVerts[patch[2]][1];
				b[2] = k0 * icosaVerts[patch[0]][2] + k1 * icosaVerts[patch[2]][2];

				for (int j = 0; j <= i; j++)
				{
					double t = i ? double(j) / double(i) : 0.0;
					double k0 = 1.0 - t;
					double k1 = t;

					double x = k0 * a[0] + k1 * b[0];
					double y = k0 * a[1] + k1 * b[1];
					double z = k0 * a[2] + k1 * b[2];

					double r = sqrt(x*x + y*y + z*z);
					double invR = 1.0 / r;
					x *= invR;
					y *= invR;
					z *= invR;

					pointIndices[numPoints++] = addPoint((float)x, (float)y, (float)z);
				}
			}

			for (int i = 0; i < segs; i++)
			{
				int index0, index1, index2;

				for (int j = 0; j < 2 * i + 1; j++)
				{
					if (j % 2 == 0)
					{
						index0 = (i*i + i) / 2 + j / 2;
						index1 = index0 + i + 1;
						index2 = index1 + 1;
					}
					else
					{
						index1 = index0 + 1;
						std::swap(index0, index2);
					}

					indices[_numIndices++] = pointIndices[index0];
					indices[_numIndices++] = pointIndices[index1];
					indices[_numIndices++] = pointIndices[index2];
				}
			}
		}

		int numIndices[] =
		{
			sizeof(indices) / sizeof(short)
		};

		pgn::VertexAttribDesc vertAttribDescs[] =
		{
			"position", pgn::FLOAT3, 0,
		};

		void* attribs[] =
		{
			pos
		};

		Geometry* geom = debug_new Geometry(subsetAllocator);

		helper.createGeometry(
			geom,
			sizeof(vertAttribDescs) / sizeof(pgn::VertexAttribDesc),
			vertAttribDescs,
			sizeof(pos) / sizeof(pos[0]),
			sizeof(numIndices) / sizeof(numIndices[0]),
			numIndices,
			attribs,
			indices
			);

		geom->primType = pgn::PrimType::TRIANGLE_LIST;
		geom->skeletonTemplate = 0;
		geom->brdfCoeffBuf.buf = 0;

		sphere = geomMgr->addResource("sphere", geom);
	}

	for (auto& resView : resViews)
		resView = 0;

	for (int i = 0; i < cfg.numActivePasses; i++)
	{
		PassEnum pass = cfg.activePasses[i];
		EnvDesc* envDesc = envDescs[pass];
		Env* env = &envs[pass];

		for (int j = 0; j < envDesc->numOffscreenRTs; j++)
		{
			RenderTargetDesc* desc = &envDesc->offscreenRTDescs[j];
			ResourceView* view = resViews[desc->resViewEnum];

			if (!view)
			{
				view = debug_new ResourceView;
				resViews[desc->resViewEnum] = view;
			}

			RenderTarget rt;
			rt.view = view;
			rt.clearNeeded = desc->clearNeeded;
			rt.clearValue[0] = desc->clearValue[0];
			rt.clearValue[1] = desc->clearValue[1];
			rt.clearValue[2] = desc->clearValue[2];
			rt.clearValue[3] = desc->clearValue[3];

			env->offscreenRTs.push_back(rt);
		}

		if (envDesc->offscreenDepthStencilBufDesc)
		{
			DepthStencilBufDesc* desc = envDesc->offscreenDepthStencilBufDesc;
			ResourceView* view = resViews[desc->resViewEnum];

			if (!view)
			{
				view = debug_new ResourceView;
				resViews[desc->resViewEnum] = view;
			}

			DepthStencilBuf rt;
			rt.view = view;
			rt.depthClearNeeded = desc->depthClearNeeded;
			rt.depthClearValue = desc->depthClearValue;
			rt.stencilClearNeeded = desc->stencilClearNeeded;
			rt.stencilClearValue = desc->stencilClearValue;

			env->depthStencilBuf = rt;
		}
		else
		{
			env->depthStencilBuf.view = 0;
		}

		int numConsts = envDesc->numConsts;

		if (numConsts)
		{
			env->consts = debug_new EnvConst*[numConsts];

			int sizeCBuf = 0;

			for (int i = 0; i < numConsts; i++)
			{
				EnvConst* envConst = &envConsts[envDesc->constEnums[i]];
				env->consts[i] = envConst;
				sizeCBuf += envConst->size;
			}

			pgn::BufferDesc bufDesc =
			{
				pgn::CONSTANT_BUFFER,
				pgn::STATIC_DRAW,
				sizeCBuf
			};

			CBufRange* bufRange = debug_new CBufRange;

			bufRange->buf = rs->createBuffer(&bufDesc);
			bufRange->offset = 0;
			bufRange->size = sizeCBuf;

			env->cbuf = bufRange;
		}

		env->numConsts = numConsts;
	}

	for (int i = 0; i < numResViews; i++)
	{
		ResourceView* view = resViews[i];

		if (!view) continue;

		ResourceViewDesc* desc = resViewDescs[i];

		view->type = desc->type;
		view->bindingPointName = desc->bindingPointName;
		view->resName = desc->resName;
		view->tex = rs->createTexture(desc->texDesc);
		view->texHandle = texMgr->addResource(view->resName, view->tex);

		switch (desc->type)
		{
		case RENDER_TARGET_VIEW:
			view->view = rs->createRenderTargetView(view->tex);
			break;
		case DEPTH_STENCIL_VIEW:
			view->view = rs->createDepthStencilView(view->tex);
			break;
		}

		view->autoSizeScale = desc->autoSizeScale;

		texBindingPointMap[view->bindingPointName] = view->tex;
	}

	bool programsReady = false;
	static char cacheFileName[] = "shaders.bin";

	if (/*cacheStream*/0)
	{
		cacheStream->open(cacheFileName, pgn::FileStream::in);

		if (cacheStream->isOpen())
		{
			long long cacheFileSize = cacheStream->getSize();
			char* cacheFileBuf = debug_new char[cacheFileSize];

			cacheStream->read(cacheFileBuf, cacheFileSize);
			cacheStream->close();

			programsReady = buildPrograms(cacheFileBuf);

			delete[] cacheFileBuf;
		}
	}

	if (!programsReady)
	{
		if (cacheStream)
			cacheStream->open(cacheFileName, pgn::FileStream::out);

		buildPrograms();

		if (cacheStream)
			cacheStream->close();
	}

	for (int i = 0; i < maxNumPrerenderedFrames; i++)
		freeList.push_back(new(_pageAlloc(sizeof(FrameContext))) FrameContext(rs));

	rs->flush();

	renderingStage = debug_new RenderingStage(this, wnd);
	pgn::PipelineStage* stage = renderingStage;
	frameQueue = pgn::Pipeline::create(sizeof(FrameContext*), maxNumPrerenderedFrames, 1, &stage, false);

	submittingCount = 0;
	finishCount = 0;
}

void Renderer::endDraw()
{
	frameQueue->destroy();
	delete renderingStage;

	for (auto frameContext : freeList)
	{
		frameContext->~FrameContext();
		pgn::pageFree(frameContext, sizeof(FrameContext));
	}

	for (auto& tech : techs)
	{
		for (auto pipe : tech.pipeSet)
		{
			if (!pipe) continue;

			pipe->program->destroy();
			if (pipe->vs) pipe->vs->destroy();
			if (pipe->ps) pipe->ps->destroy();
			for (auto sampler : pipe->samplers)
				sampler->destroy();
			pipe->rasterizerState->destroy();
			pipe->depthStencilState->destroy();
			pipe->outputMergerState->destroy();

			for (auto& entry : pipe->vertexInputLayoutMap)
				entry.second.p->destroy();

			delete pipe;
		}
	}

	for (int i = 0; i < cfg.numActivePasses; i++)
	{
		PassEnum pass = cfg.activePasses[i];
		Env* env = &envs[pass];

		if (env->numConsts)
		{
			delete env->consts;
			env->cbuf->buf->destroy();
			delete env->cbuf;
		}
	}

	for (auto view : resViews)
	{
		if (!view) continue;

		texMgr->removeResource(view->texHandle);
		view->view->destroy();
		view->tex->destroy();
		delete view;
	}

	// 删除灰色纹理
	{
		pgn::Texture* tex = (pgn::Texture*)gray->core();
		texMgr->removeResource(gray);
		tex->destroy();
	}

	GeometryHelper helper(rs);

	// 删除screen-aligned quad
	{
		Geometry* geom = (Geometry*)screenAlignedQuad->core();
		geomMgr->removeResource(screenAlignedQuad);
		helper.destroyGeometry(geom);
		delete geom;
	}

	// 删除球体
	{
		Geometry* geom = (Geometry*)sphere->core();
		geomMgr->removeResource(sphere);
		helper.destroyGeometry(geom);
		delete geom;
	}

	geomMgr->destroy();
	editableGeomMgr->destroy();
	navGeomMgr->destroy();
	texMgr->destroy();
	pgn::destroyAsyncLoader(resLoader);

	rs->destroy();
	rc->endDraw();
}

FrameContext* Renderer::beginSubmit()
{
	FrameContext** ppFrameContext = (FrameContext**)frameQueue->get();

	if (ppFrameContext)
		retired.push(*ppFrameContext);

	while (!retired.empty())
	{
		FrameContext* frameContext = retired.front();

		if (!rs->checkSyncPoint(frameContext->sync))
			break;

		freeList.push_back(frameContext);
		finishCount++;

		retired.pop();
	}

	if (freeList.empty())
		return 0;

	frameContext = freeList.back();
	freeList.pop_back();

	frameContext->cbufAllocator->init();

	return frameContext;
}

void Renderer::submit(PassEnum passEnum, TechEnum techEnum, Batch* batch)
{
	if (!isPassActive[passEnum]) return;

	BatchGroupPtr* ppBatchGroup = &frameContext->batches[passEnum][techEnum][batch->geom->vertexFormat];
	BatchGroup* batchGroup = ppBatchGroup->p;
	if (!batchGroup)
	{
		batchGroup = debug_new BatchGroup(heap);
		batchGroup->get_allocator().setPool(frameContext->heap);
		ppBatchGroup->p = batchGroup;
	}
	batchGroup->push_back(*batch);
}

void Renderer::endSubmit()
{
	frameContext->cbufAllocator->commit();
	rs->flush();

	frameQueue->put(&frameContext);
	submittingCount++;
}

void Renderer::render(FrameContext* frameContext)
{
	Viewport viewport = frameContext->viewport;

	for (auto view : resViews)
	{
		if (!view) continue;

		if (view->autoSizeScale > 0.0f)
		{
			int width = (int)(viewport.width * view->autoSizeScale);
			int height = (int)(viewport.height * view->autoSizeScale);

			pgn::TextureDesc texDesc = *view->tex->getDesc();

			if (texDesc.width != width || texDesc.height != height)
			{
				view->view->destroy();
				view->tex->destroy();

				texDesc.width = width;
				texDesc.height = height;

				view->tex = rs->createTexture(&texDesc);
				view->texHandle = texMgr->addResource(view->resName, view->tex);

				switch (view->type)
				{
				case RENDER_TARGET_VIEW:
					view->view = rs->createRenderTargetView(view->tex);
					break;
				case DEPTH_STENCIL_VIEW:
					view->view = rs->createDepthStencilView(view->tex);
					break;
				}

				texBindingPointMap[view->bindingPointName] = view->tex;
			}
		}
	}

	pgn::Float4x3 view = frameContext->view;
	pgn::Float4x4 proj = frameContext->proj;

	pgn::Float4x4 view4x4;
	view4x4.float4x3 = view;
	view4x4[3][0] = 0;
	view4x4[3][1] = 0;
	view4x4[3][2] = 0;
	view4x4[3][3] = 1;

	pgn::Float4x4 viewProj;
	pgn::mul(&view4x4, &proj, &viewProj);

	pgn::Float4x4 invProj =
	{
		1 / proj[0][0],		0,					0,					0,
		0,					1 / proj[1][1],		0,					0,
		0,					0,					0,					1,
		0,					0,					1 / proj[2][3],		-proj[2][2] / proj[2][3]
	};

	pgn::Float4x3 invView;
	pgn::calculateInverseViewMat(&view, &invView);

	pgn::Float3 camPos;
	camPos.x = invView[0][3];
	camPos.y = invView[1][3];
	camPos.z = invView[2][3];

	for (int i = 0; i < frameContext->numPointLights; i++)
	{
		frameContext->vPointLights[i].intensity_spec = frameContext->wPointLights[i].intensity_spec;
		frameContext->vPointLights[i].pos_att[3] = frameContext->wPointLights[i].pos_att[3];
		pgn::transformVertex(&frameContext->wPointLights[i].pos_att.float3, &view, &frameContext->vPointLights[i].pos_att.float3);
	}

	for (int i = 0; i < FrameContext::maxNumDirLights; i++)
	{
		if (frameContext->wDirLights[i].dir_enabled[3] == 0.0f) continue;

		frameContext->vDirLights[i].intensity_spec = frameContext->wDirLights[i].intensity_spec;
		frameContext->vDirLights[i].dir_enabled[3] = frameContext->wDirLights[i].dir_enabled[3];
		pgn::transformVector(&frameContext->wDirLights[i].dir_enabled.float3, &view, &frameContext->vDirLights[i].dir_enabled.float3);
	}

	envConsts[VIEW].p = &view;
	envConsts[VIEW_PROJ].p = &viewProj;
	envConsts[INV_PROJ].p = &invProj;
	envConsts[CAM_POS].p = &camPos;
	envConsts[W_POINT_LIGHT].p = frameContext->wPointLights;
	envConsts[V_POINT_LIGHT].p = frameContext->vPointLights;
	envConsts[W_DIR_LIGHT].p = frameContext->wDirLights;
	envConsts[V_DIR_LIGHT].p = frameContext->vDirLights;

	enum BindingPoint
	{
		  envCBlockBindingPoint
		, instanceCBlockBindingPoint
		, boneMatBlockBindingPoint
		, materialBlockBindingPoint
	};

	rs->clearRenderTargetView(0, 0.0f, 0.0f, 0.0f, 1.0f);
	rs->clearDepthStencilView(0, true, 1.0f, false, 0);

	for (int i = 0; i < cfg.numActivePasses; i++)
	{
		PassEnum pass = cfg.activePasses[i];
		Env* env = &envs[pass];
		CBufRange* cbuf = env->cbuf;

		if (env->numConsts)
		{
			char* p = (char*)cbuf->buf->map(cbuf->offset, cbuf->size);
			for(int j = 0; j < env->numConsts; j++)
			{
				EnvConst* envConst = env->consts[j];
				memcpy(p, envConst->p, envConst->size);
				p += envConst->size;
			}
			cbuf->buf->unmap();
			rs->setConstantBuffers(envCBlockBindingPoint, 1, &cbuf->buf, &cbuf->offset, &cbuf->size);
		}

		if (!env->offscreenRTs.empty() || env->depthStencilBuf.view)
		{
			pgn::RenderTargetView* rtViews[pgn::maxNumRenderTargets];

			for (unsigned j = 0; j < env->offscreenRTs.size(); j++)
			{
				RenderTarget* rt = &env->offscreenRTs[j];
				pgn::RenderTargetView* rtView = (pgn::RenderTargetView*)rt->view->view;

				rtViews[j] = rtView;

				if (rt->clearNeeded)
					rs->clearRenderTargetView(rtView, rt->clearValue[0], rt->clearValue[1], rt->clearValue[2], rt->clearValue[3]);
			}

			if (env->depthStencilBuf.depthClearNeeded || env->depthStencilBuf.stencilClearNeeded)
				rs->clearDepthStencilView((pgn::DepthStencilView*)env->depthStencilBuf.view->view, env->depthStencilBuf.depthClearNeeded, env->depthStencilBuf.depthClearValue, env->depthStencilBuf.stencilClearNeeded, env->depthStencilBuf.stencilClearValue);

			ResourceView* rtView = env->offscreenRTs.size() ? env->offscreenRTs[0].view : env->depthStencilBuf.view;
			pgn::TextureDesc* texDesc = rtView->tex->getDesc();

			rs->beginOffscreenPass((int)env->offscreenRTs.size(), rtViews, (pgn::DepthStencilView*)env->depthStencilBuf.view->view);
			rs->setViewport(0, 0, texDesc->width, texDesc->height, texDesc->height, 0.0f, 1.0f);
		}
		else
		{
			rs->beginOnscreenPass();
			rs->setViewport(viewport.left, viewport.top, viewport.width, viewport.height, viewport.fullHeight, 0.0f, 1.0f);
		}

		auto& passBatches = frameContext->batches[pass];
		for (auto& techBatches : passBatches)
		{
			TechEnum techEnum = techBatches.first;
			PipeState* pipe = techs[techEnum].pipeSet[pass];

			if (!pipe) continue;

			rs->setProgram(pipe->program);

			if (!pipe->samplers.empty())
				rs->setSamplers(0, (int)pipe->samplers.size(), &pipe->samplers[0]);

			rs->setRasterizerState(pipe->rasterizerState);
			rs->setDepthStencilState(pipe->depthStencilState, 0);
			rs->setOutputMergerState(pipe->outputMergerState);

			BatchGroupMap& batchGroupMap = techBatches.second;

			for (auto& batchGroupMapEntry : batchGroupMap)
			{
				const VertexFormat* vf = batchGroupMapEntry.first;

				VertexInputLayoutPtr* ppVertexInputLayout = &pipe->vertexInputLayoutMap[vf];
				pgn::VertexInputLayout* vertexInputLayout = ppVertexInputLayout->p;
				if(!vertexInputLayout)
				{
					vertexInputLayout = rs->createVertexInputLayout(pipe->program, vf->numAttribs, vf->vertexAttribDescs, vf->strides);
					ppVertexInputLayout->p = vertexInputLayout;
				}
				rs->setVertexInputLayout(vertexInputLayout);

				BatchGroup& batchGroup = *batchGroupMapEntry.second.p;

				for (auto& batch : batchGroup)
				{
					Geometry* geom = batch.geom;

					rs->setConstantBuffers(instanceCBlockBindingPoint, 1, &batch.instanceCBlockBuf.buf, &batch.instanceCBlockBuf.offset, &batch.instanceCBlockBuf.size);

					if (batch.boneMatBuf.buf)
						rs->setConstantBuffers(boneMatBlockBindingPoint, 1, &batch.boneMatBuf.buf, &batch.boneMatBuf.offset, &batch.boneMatBuf.size);

					if (geom->brdfCoeffBuf.buf)
						rs->setConstantBuffers(materialBlockBindingPoint, 1, &geom->brdfCoeffBuf.buf, &geom->brdfCoeffBuf.offset, &geom->brdfCoeffBuf.size);

					rs->setPrimitiveType(geom->primType);
					rs->setVertexBuffers(0, geom->vertexFormat->numAttribs, geom->vertexBuffers, geom->offsets);

					TextureInfo* textureInfo = batch.textureInfo;
					std::list<TextureSet, TextureSetAllocator>::iterator itTexSet;

					if (textureInfo)
						itTexSet = textureInfo->textureSets.begin();

					for (auto subset : geom->subsets)
					{
						if (textureInfo)
						{
							pgn::ResourceHandle** textures = itTexSet->textures;

							for (int j = 0; j < maxTexturesPerSubset; j++)
								if (textures[j]) *textureInfo->texBindingPoints[j] = (pgn::Texture*)textures[j]->core();

							itTexSet++;
						}

						pgn::Texture* textures[pgn::maxNumTextures];

						assert(pipe->texBindingPoints.size() < pgn::maxNumTextures);

						for (unsigned j = 0; j < pipe->texBindingPoints.size(); j++)
							textures[j] = *pipe->texBindingPoints[j];

						rs->setTextures(0, (int)pipe->texBindingPoints.size(), textures);
						rs->setIndexBuffer(geom->indexBuffer, subset.indexBufferOffset);
						rs->draw(subset.numIndices, batch.instanceCount);
					}
				}

				//batchGroup.clear();
			}

			for (auto& batchGroupMapEntry : batchGroupMap)
			{
				BatchGroup& batchGroup = *batchGroupMapEntry.second.p;
				batchGroup.clear();
			}
		}

		rs->endPass(i == cfg.numActivePasses - 1 ? frameContext->sync : 0);
	}

	frameContext->heap->clear();
}

void Renderer::finish()
{
	frameQueue->finish();

	while (FrameContext** ppFrameContext = (FrameContext**)frameQueue->get())
	{
		retired.push(*ppFrameContext);
	}

	while (!retired.empty())
	{
		FrameContext* frameContext = retired.front();

		freeList.push_back(frameContext);
		finishCount++;

		retired.pop();
	}
}
