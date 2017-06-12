#include <PGN/Assets/PNM.h>
#include <PGN/Common/debug_new.h>
#include <PGN/Utilities/HeapAllocator.h>
#include <PGN/Utilities/ResourceManager/ResourceHandle.h>
#include <PGN/Utilities/ResourceManager/ResourceManager.h>
#include <PGN/Utilities/SkeletalAnimation/Skeleton.h>
#include <PGN/Utilities/SkeletalAnimation/SkeletonTemplate.h>
#include "Camera.h"
#include "DirectionalLight.h"
#include "EditableModel.h"
#include "Graphics.h"
#include "Model.h"
#include "NavModel.h"
#include "PointLight.h"
#include "Renderer/CBufAllocator.h"
#include "Renderer/Movable2D.h"
#include "Scene.h"
#include "SkeletalModel.h"

Graphics::Graphics(pgn::Display displayPrototype, pgn::FileStream* assetStream, pgn::FileStream* cacheStream)
	: renderer(displayPrototype, assetStream, cacheStream)
{
	modelPool = pgn::Pool::create(sizeof(Model));
	editableModelPool = pgn::Pool::create(sizeof(EditableModel));
	skeletalModelPool = pgn::Pool::create(sizeof(SkeletalModel));
	navModelPool = pgn::Pool::create(sizeof(NavModel));
	pointLightPool = pgn::Pool::create(sizeof(PointLight));
	dirLightPool = pgn::Pool::create(sizeof(DirectionalLight));
	tmpBuf = pgn::Heap::create();
}

void Graphics::dispose()
{
	modelPool->destroy();
	editableModelPool->destroy();
	skeletalModelPool->destroy();
	navModelPool->destroy();
	pointLightPool->destroy();
	dirLightPool->destroy();
	tmpBuf->destroy();
}

pgn::Graphics* pgn::Graphics::create(pgn::Display displayPrototype, pgn::FileStream* assetStream, pgn::FileStream* cacheStream)
{
	return debug_new ::Graphics(displayPrototype, assetStream, cacheStream);
}

void Graphics::_free()
{
	delete this;
}

void Graphics::beginDraw(pgn::Window* wnd)
{
	renderer.beginDraw(wnd, &lightIndexedDeferredRendering);
	//renderer.beginDraw(wnd, &lightIndexedForwardRendering);
}

bool Graphics::beginFrame()
{
	return renderer.beginFrame();
}

struct SceneEntityListEntry
{
	SceneEntity* sceneEntity;
	SceneEntityListEntry* next;
};

class SceneEntityList
{
public:
	SceneEntityListEntry* first;
	int size;

	SceneEntityList()
	{
		size = 0;
	}
};

#ifndef min
#define min(a,b) (a < b ? a : b)
#endif

const int maxInstanceCount = 256;

void submitSkyBoxes(Graphics* graphics, SceneEntityListEntry* first, int count, CBufAllocator* cbufAllocator)
{
	assert(count <= maxInstanceCount);

	Model* model = first->sceneEntity->model;

	if (!model->complete())
		return;

	model->submittingStamp = graphics->renderer.submittingCount;

	Batch batch;
	batch.geom = (Geometry*)model->geomHandle->core();
	batch.textureInfo = &model->textureInfo;
	batch.boneMatBuf.buf = 0;

	SceneEntityListEntry* entry = first;
	int n = count;

	while (n)
	{
		batch.instanceCount = min(n, maxInstanceCount);
		Movable* instances = (Movable*)cbufAllocator->alloc(sizeof(Movable) * batch.instanceCount, &batch.instanceCBlockBuf);

		for (int i = 0; i < batch.instanceCount; i++)
		{
			instances[i] = entry->sceneEntity->movable;
			entry = entry->next;
		}

		graphics->renderer.submit(SKY_BOX_RENDERING_PASS, SKY_BOX_TECH, &batch);

		n -= batch.instanceCount;
	}
}

void submitModels(Graphics* graphics, SceneEntityListEntry* first, int count, CBufAllocator* cbufAllocator)
{
	assert(count <= maxInstanceCount);

	Model* model = first->sceneEntity->model;

	if (!model->complete())
		return;

	model->submittingStamp = graphics->renderer.submittingCount;

	Batch batch;
	batch.geom = (Geometry*)model->geomHandle->core();
	batch.textureInfo = &model->textureInfo;
	batch.boneMatBuf.buf = 0;

	SceneEntityListEntry* entry = first;
	int n = count;

	while (n)
	{
		batch.instanceCount = min(n, maxInstanceCount);
		Movable* instances = (Movable*)cbufAllocator->alloc(sizeof(Movable) * batch.instanceCount, &batch.instanceCBlockBuf);

		for (int i = 0; i < batch.instanceCount; i++)
		{
			instances[i] = entry->sceneEntity->movable;
			entry = entry->next;
		}

		for (int i = 0; i < graphics->renderer.cfg.numOpaqueEntityPasses; i++)
		{
			graphics->renderer.submit(graphics->renderer.cfg.opaqueEntityPasses[i], STATIC_MESH_TECH, &batch);
		}

		n -= batch.instanceCount;
	}
}

void submitSkeletalModels(Graphics* graphics, SceneEntityListEntry* first, int count, CBufAllocator* cbufAllocator)
{
	assert(count <= maxInstanceCount);

	SkeletalModel* skeletalModel = first->sceneEntity->skeletalModel;
	Model* model = skeletalModel->model;

	if (!model->complete())
		return;

	model->submittingStamp = graphics->renderer.submittingCount;

	Batch batch;
	batch.geom = (Geometry*)model->geomHandle->core();
	batch.textureInfo = &model->textureInfo;

	TechEnum tech;

	pgn::SkeletonTemplate* skelTempl = batch.geom->skeletonTemplate;
	if (skelTempl)
	{
		tech = SKINNED_MESH_TECH;

		int numBones = skelTempl->getNumBones();
		pgn::Float4x3* boneMats = (pgn::Float4x3*)cbufAllocator->alloc(sizeof(skeletalModel->boneMats[0]) * numBones, &batch.boneMatBuf);

		if (skeletalModel->boneMats)
		{
			for (int j = 0; j < numBones; j++)
				boneMats[j] = skeletalModel->boneMats[j];
		}
		else
		{
			skeletalModel->skel->updatePose(0, skelTempl, boneMats);
		}
	}
	else
	{
		tech = STATIC_MESH_TECH;
		batch.boneMatBuf.buf = 0;
	}

	SceneEntityListEntry* entry = first;
	int n = count;

	while (n)
	{
		batch.instanceCount = min(n, maxInstanceCount);
		Movable* instances = (Movable*)cbufAllocator->alloc(sizeof(Movable) * batch.instanceCount, &batch.instanceCBlockBuf);

		for (int i = 0; i < batch.instanceCount; i++)
		{
			instances[i] = entry->sceneEntity->movable;
			entry = entry->next;
		}

		for (int i = 0; i < graphics->renderer.cfg.numOpaqueEntityPasses; i++)
		{
			graphics->renderer.submit(graphics->renderer.cfg.opaqueEntityPasses[i], tech, &batch);
		}

		n -= batch.instanceCount;
	}
}

void submitNavModels(Graphics* graphics, SceneEntityListEntry* first, int count, CBufAllocator* cbufAllocator)
{
	assert(count <= maxInstanceCount);

	NavModel* model = first->sceneEntity->navModel;

	if (!model->complete())
		return;

	model->submittingStamp = graphics->renderer.submittingCount;

	Batch batch;
	batch.geom = (Geometry*)model->geomHandle->core();
	batch.textureInfo = 0;
	batch.boneMatBuf.buf = 0;

	SceneEntityListEntry* entry = first;
	int n = count;

	while (n)
	{
		batch.instanceCount = min(n, maxInstanceCount);
		Movable* instances = (Movable*)cbufAllocator->alloc(sizeof(Movable) * batch.instanceCount, &batch.instanceCBlockBuf);

		for (int i = 0; i < batch.instanceCount; i++)
		{
			instances[i] = entry->sceneEntity->movable;
			entry = entry->next;
		}

		for (int i = 0; i < graphics->renderer.cfg.numTransparentEntityPasses; i++)
		{
			graphics->renderer.submit(graphics->renderer.cfg.transparentEntityPasses[i], NAV_MESH_TECH, &batch);
		}

		n -= batch.instanceCount;
	}
}

void Graphics::draw(pgn::Scene* _scene, pgn::Camera* _camera)
{
	Scene* scene = (Scene*)_scene;
	Camera* camera = (Camera*)_camera;

	SceneContext* sceneContext = renderer.beginSubmit();

	performPendingRemovals();

	CBufAllocator* cbufAllocator = sceneContext->cbufAllocator;

	typedef void Submit(Graphics* graphics, SceneEntityListEntry* first, int count, CBufAllocator* cbufAllocator);

	auto groupSubmit = [=](SceneEntity::StdList& list, Submit submit)
	{
		typedef pgn::HeapAllocator<std::pair<void*, SceneEntityList>> SceneEntityGroupAllocator;
		typedef std::map<void*, SceneEntityList, std::less<void*>, SceneEntityGroupAllocator> SceneEntityGroupMap;

		SceneEntityGroupMap* _sceneEntityGroupMap = new(tmpBuf->alloc(sizeof(SceneEntityGroupMap))) SceneEntityGroupMap(tmpBuf);
		SceneEntityGroupMap& sceneEntityGroupMap = *_sceneEntityGroupMap;

		for (auto& _sceneEntity : list)
		{
			SceneEntity* sceneEntity = (SceneEntity*)&_sceneEntity;
			if (!sceneEntity->useInstancedDrawing)
			{
				SceneEntityListEntry entry;
				entry.sceneEntity = sceneEntity;
				submit(this, &entry, 1, cbufAllocator);
			}
			else
			{
				SceneEntityList* sceneEntityList = &sceneEntityGroupMap[sceneEntity->skeletalModel];
				SceneEntityListEntry* entry = (SceneEntityListEntry*)tmpBuf->alloc(sizeof(SceneEntityListEntry));
				entry->sceneEntity = sceneEntity;
				entry->next = sceneEntityList->first;
				sceneEntityList->first = entry;
				sceneEntityList->size++;
			}
		}

		for (auto& entry : sceneEntityGroupMap)
		{
			SceneEntityList sceneEntityList = entry.second;
			submit(this, sceneEntityList.first, sceneEntityList.size, cbufAllocator);
		}
	};

	groupSubmit(scene->sceneSkyBoxes, submitSkyBoxes);
	groupSubmit(scene->sceneModels, submitModels);
	groupSubmit(scene->sceneSkeletalModels, submitSkeletalModels);
	groupSubmit(scene->sceneNavModels, submitNavModels);

	tmpBuf->clear();

	for (int i = 0; i < SceneContext::maxNumDirLights; i++)
		sceneContext->wDirLights[i].dir_enabled[3] = 0.0f;

	assert(scene->sceneDirLights.size() <= SceneContext::maxNumDirLights);

	{
		int i = 0;
		for (auto& sceneDirLight : scene->sceneDirLights)
		{
			sceneContext->wDirLights[i].intensity_spec = sceneDirLight.dirLight->intensity_spec;
			sceneContext->wDirLights[i].dir_enabled.float3 = sceneDirLight.dir;
			sceneContext->wDirLights[i].dir_enabled[3] = 1.0f;
			i++;
		}
	}

	ScenePointLight* scenePointLightArray[SceneContext::maxNumPointLights];
	_aligned(16, float pos[3][SceneContext::maxNumPointLights]);
	_aligned(16, float r[SceneContext::maxNumPointLights]);
	_aligned(16, float x[SceneContext::maxNumPointLights * 2]);
	_aligned(16, float y[SceneContext::maxNumPointLights * 2]);
	_aligned(16, float z[SceneContext::maxNumPointLights * 2]);

	float* minX = x;
	float* maxX = x + SceneContext::maxNumPointLights;
	float* minY = y;
	float* maxY = y + SceneContext::maxNumPointLights;
	float* minZ = z;
	float* maxZ = z + SceneContext::maxNumPointLights;

	{
		int i = 0;
		for (auto& scenePointLight : scene->scenePointLights)
		{
			if (i >= SceneContext::maxNumPointLights)
				break;

			scenePointLightArray[i] = &scenePointLight;
			pos[0][i] = scenePointLight.pos.x;
			pos[1][i] = scenePointLight.pos.y;
			pos[2][i] = scenePointLight.pos.z;
			r[i] = scenePointLight.pointLight->radius;
			i++;
		}

		int numScenePointLights = i;

		int n = numScenePointLights / 4;
		if (numScenePointLights % 4) n++;

		pgn::sub((pgn::Float4*)pos[0], (pgn::Float4*)r, (pgn::Float4*)minX, n);
		pgn::add((pgn::Float4*)pos[0], (pgn::Float4*)r, (pgn::Float4*)maxX, n);
		pgn::sub((pgn::Float4*)pos[1], (pgn::Float4*)r, (pgn::Float4*)minY, n);
		pgn::add((pgn::Float4*)pos[1], (pgn::Float4*)r, (pgn::Float4*)maxY, n);
		pgn::sub((pgn::Float4*)pos[2], (pgn::Float4*)r, (pgn::Float4*)minZ, n);
		pgn::add((pgn::Float4*)pos[2], (pgn::Float4*)r, (pgn::Float4*)maxZ, n);

		for (auto& set : scene->lightDisjointSets)
			set.clear();

		unsigned char disjointSetMasks[SceneContext::maxNumPointLights];
		memset(disjointSetMasks, 0, sizeof(disjointSetMasks));

		for (int a = 0; a < numScenePointLights; a++)
		{
			unsigned char mask = 0;

			for (int i = 0; i < 4; i++)
			{
				if ((disjointSetMasks[a] & (1 << i)) == 0)
				{
					scene->lightDisjointSets[i].push_back(scenePointLightArray[a]);
					mask = 1 << i;
					break;
				}
			}

			//if (!mask) continue;

			for (int b = a + 1; b < numScenePointLights; b++)
			{
				if (minX[a] > maxX[b] || maxX[a] < minX[b])	continue;
				if (minY[a] > maxY[b] || maxY[a] < minY[b])	continue;
				if (minZ[a] > maxZ[b] || maxZ[a] < minZ[b])	continue;

				float dx = pos[0][a] - pos[0][b];
				float dy = pos[1][a] - pos[1][b];
				float dz = pos[2][a] - pos[2][b];
				float R = r[a] + r[b];

				if (dx*dx + dy*dy + dz*dz > R*R)
					continue;

				disjointSetMasks[b] |= mask;
			}
		}
	}

	Batch batch;

	batch.geom = (Geometry*)renderer.geomMgr->peekResource("sphere");
	batch.textureInfo = 0;
	batch.boneMatBuf.buf = 0;

	{
		int i = 0;

		for (int j = 0; j < Scene::numLightDisjointSets; j++)
		{
			auto& set = scene->lightDisjointSets[j];
			auto it = set.begin();

			for (int count = (int)set.size(); count > 0; count -= batch.instanceCount)
			{
				struct PointLightVolumnInstance
				{
					pgn::Float4 pos_scale;
					int lightIndex;
				};

				batch.instanceCount = min(count, maxInstanceCount);
				PointLightVolumnInstance* instances = (PointLightVolumnInstance*)cbufAllocator->alloc(sizeof(PointLightVolumnInstance) * batch.instanceCount, &batch.instanceCBlockBuf);

				for (int k = 0; k < batch.instanceCount; k++)
				{
					ScenePointLight* scenePointLight = *it++;
					PointLight* light = scenePointLight->pointLight;

					instances[k].pos_scale.float3 = scenePointLight->pos;
					instances[k].pos_scale[3] = light->radius;
					instances[k].lightIndex = i;

					sceneContext->wPointLights[i].intensity_spec = light->intensity_spec;
					sceneContext->wPointLights[i].pos_att.float3 = scenePointLight->pos;
					sceneContext->wPointLights[i].pos_att[3] = light->att;

					i++;
				}

				int pass = (LIGHT_VOLUME_BACK_FACE_PASS_2 - LIGHT_VOLUME_BACK_FACE_PASS_1) * j;

				renderer.submit((PassEnum)(LIGHT_VOLUME_BACK_FACE_PASS_1 + pass), LIGHT_INDEXING_TECH, &batch);
				renderer.submit((PassEnum)(LIGHT_VOLUME_FRONT_FACE_PASS_1 + pass), LIGHT_INDEXING_TECH, &batch);
				renderer.submit((PassEnum)(LIGHT_INDEXING_PASS_1 + pass), LIGHT_INDEXING_TECH, &batch);
			}
		}

		sceneContext->numPointLights = i;
	}

	batch.geom = (Geometry*)renderer.geomMgr->peekResource("screen-aligned quad");
	batch.textureInfo = 0;
	batch.instanceCount = 1;

	static pgn::Float3x2 I =
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	Movable2D* movable2D = (Movable2D*)cbufAllocator->alloc(sizeof(Movable2D), &batch.instanceCBlockBuf);
	movable2D->transf = I;
	movable2D->scaleX = 1.0f;
	movable2D->scaleY = 1.0f;

	for (int i = 0; i < renderer.cfg.numPostProcessingPasses; i++)
	{
		renderer.submit(renderer.cfg.postProcessingPasses[i], POST_PROCESSING_TECH, &batch);
	}

	sceneContext->view = camera->viewMat;
	sceneContext->proj = camera->projMat;
	sceneContext->viewport = camera->viewport;

	renderer.endSubmit();
}

void Graphics::endDraw()
{
	renderer.finish();
	performPendingRemovals();
	renderer.endDraw();
}

void Graphics::endFrame()
{
	return renderer.endFrame();
}

void Graphics::performPendingRemovals()
{
	while (!pendingModelRemovals.empty())
	{
		Model* model = pendingModelRemovals.front();

		if (model->submittingStamp >= renderer.retireCount) break;

		model->~Model();
		modelPool->_free(model);
		pendingModelRemovals.pop_front();
	}

	while (!pendingEditableModelRemovals.empty())
	{
		EditableModel* model = pendingEditableModelRemovals.front();

		if (model->submittingStamp >= renderer.retireCount) break;

		model->~EditableModel();
		editableModelPool->_free(model);
		pendingEditableModelRemovals.pop_front();
	}

	while (!pendingNavModelRemovals.empty())
	{
		NavModel* model = pendingNavModelRemovals.front();

		if (model->submittingStamp >= renderer.retireCount) break;

		model->~NavModel();
		navModelPool->_free(model);
		pendingNavModelRemovals.pop_front();
	}
}
