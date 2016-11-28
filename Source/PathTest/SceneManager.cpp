#include <PGN/FileStream/StdFileStream.h>
#include <PGN/Graphics/Camera.h>
#include <PGN/Graphics/Entity.h>
#include <PGN/Graphics/Graphics.h>
#include <PGN/Graphics/NavModel.h>
#include <PGN/Graphics/Model.h>
#include <PGN/Graphics/PointLight.h>
#include <PGN/Graphics/Scene.h>
#include <PGN/Graphics/SceneEntity.h>
#include <PGN/Graphics/ScenePointLight.h>
#include <PGN/Math/Math.h>
#include <PGN/Platform/DebugHeap.h>
#include <PGN/Platform/Graphics/Window.h>
#include <PGN/Utilities/SkeletalAnimation/Animation.h>
#include <PGN/Utilities/SkeletalAnimation/AnimationFactory.h>
#include <PGN/Utilities/SkeletalAnimation/Skeleton.h>
#include <PGN/Utilities/SkeletalAnimation/SkeletonFactory.h>
#include <PGN/Utilities/SkeletalAnimation/SkeletonTemplate.h>
#include <PGN/Utilities/Clock.h>
#include "SceneManager.h"

SceneManager::SceneManager() 
{
	window = NULL;
	assetStream = NULL;
	cacheStream = NULL;
	graphics = NULL;
	scene = NULL;
	camera = NULL;
	animStream = NULL;
	skelFactory = NULL;
	animFactory = NULL;
}

SceneManager::~SceneManager() 
{
}

void SceneManager::init(pgn::Window* wnd) 
{
	window = wnd;
	assetStream = pgn::createStdFileStream("");
	cacheStream = pgn::createStdFileStream("");
	graphics = pgn::Graphics::create(window->getDisplay(), assetStream, cacheStream);
	scene = graphics->createScene();
	camera = graphics->createCamera();
	camera->setFrustumFovLH(60.0f / 180.0f*3.14f, 4.0f / 3.0f, 0.01f, 1024);

	animStream = pgn::createStdFileStream("");
	skelFactory = pgn::SkeletonFactory::create();
	animFactory = pgn::AnimationFactory::create(animStream);

	graphics->beginDraw(window);
}

void SceneManager::destroy() 
{
	for (auto& light : scenePointLights)
	{
		scene->remove(light);
	}
	for (auto& entity : sceneEntities)
	{
		scene->remove(entity);
	}

	scene->destroy();
	camera->destroy();

	for (auto& light : pointLights)
	{
		light->destroy();
	}
	for (auto& entity : entities)
	{
		entity->destroy();
	}
	for (auto& navModel : navModels)
	{
		navModel->destroy();
	}
	for (auto& model : models)
	{
		model->destroy();
	}
	for (auto& skeleton : skeletons)
	{
		skeleton->destroy();
	}
	for (auto& anim : animations)
	{
		anim->destroy();
	}
	
	skelFactory->destroy();
	animFactory->destroy();
	graphics->endDraw();
	graphics->destroy();

	pgn::destroyStdFileStream(animStream);
	pgn::destroyStdFileStream(assetStream);
	pgn::destroyStdFileStream(cacheStream);
}

void SceneManager::tick(int dt)
{
	for (auto& skeleton : skeletons)
	{
		skeleton->updatePose(dt);
	}
	
	camera->setViewport(0, 0, window->getClientWidth(), window->getClientHeight(), window->getClientHeight());
	scene->commit(camera);
}

void SceneManager::setCameraViewMat(pgn::Float4x3& viewMat)
{
	camera->setViewMat(&viewMat);
}

void SceneManager::screenPointToRay(pgn::Float3& begin, pgn::Float3& end, float x, float y)
{
	camera->screenPointToRay(begin, end, x, y);
}

void SceneManager::addPointLight(pgn::Float4 intensity, float radius, pgn::Float3 pos)
{
	pgn::PointLight* light = graphics->createPointLight();
	light->setIntensity(intensity[0], intensity[1], intensity[2], intensity[3]);
	light->setRadius(radius);
	pgn::ScenePointLight* sceneLight = scene->add(light);
	sceneLight->setPosition(&pos);
	pointLights.push_back(light);
	scenePointLights.push_back(sceneLight);
}

pgn::NavModel* SceneManager::addNavModel(char* nav)
{
	pgn::NavModel* model = graphics->createNavModel();
	model->setMesh(nav);
	navModels.push_back(model);
	return model;
}

pgn::Model* SceneManager::addModel(char* pnm, pgn::Float4x3& worldMat, float scale)
{
	pgn::Model* model = graphics->createModel();
	model->setMesh(pnm);
	model->setDiffuseMap(0, "gray");
	pgn::Entity* entity = graphics->createEntity();
	entity->setModel(model);

	pgn::SceneEntity* sceneEntity = scene->add(entity, false);
	sceneEntity->setScale(scale, scale);
	sceneEntity->setWorldMat(&worldMat);

	models.push_back(model);
	entities.push_back(entity);
	sceneEntities.push_back(sceneEntity);
	return model;
}

pgn::SceneEntity* SceneManager::addCharacter(char* pnm, pgn::Float4x3& worldMat, float scale)
{
	pgn::Entity* entity = graphics->createEntity();
	pgn::Model* model = graphics->createModel();
	model->setMesh(pnm);
	model->setDiffuseMap(0, "res/casual02_f_25.etc2.pnt");
	entity->setModel(model);

	pgn::Skeleton* skel = skelFactory->createSkeleton();
	pgn::Animation* anim = animFactory->createAnimation();
	anim->set("res/idle1_f.pna");
	skel->playAnimation(anim);
	entity->setSkeleton(skel);
	pgn::SceneEntity* sceneEntity = scene->add(entity, false);
	sceneEntity->setScale(scale, scale);
	sceneEntity->setWorldMat(&worldMat);

	models.push_back(model);
	skeletons.push_back(skel);
	animations.push_back(anim);
	entities.push_back(entity);
	sceneEntities.push_back(sceneEntity);
	return sceneEntity;
}
