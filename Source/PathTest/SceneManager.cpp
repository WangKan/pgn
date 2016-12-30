#include <PGN/FileStream/StdFileStream.h>
#include <PGN/Graphics/Camera.h>
#include <PGN/Graphics/Entity.h>
#include <PGN/Graphics/Graphics.h>
#include <PGN/Graphics/Model.h>
#include <PGN/Graphics/PointLight.h>
#include <PGN/Graphics/Scene.h>
#include <PGN/Graphics/SceneEntity.h>
#include <PGN/Graphics/ScenePointLight.h>
#include <PGN/Math/Math.h>
#include <PGN/Platform/UI/Window.h>
#include <PGN/Utilities/LinearTransformations.h>
#include <PGN/Utilities/PathFinding/PathFinder.h>
#include <PGN/Utilities/PathFinding/PathFinderFactory.h>
#include <PGN/Utilities/Physics/Physics.h>
#include <PGN/Utilities/Physics/PhysicsFactory.h>
#include <PGN/Utilities/Physics/PhysicsWorld.h>
#include <PGN/Utilities/Physics/PhysicsWorldFactory.h>
#include <PGN/Utilities/SkeletalAnimation/Animation.h>
#include <PGN/Utilities/SkeletalAnimation/AnimationFactory.h>
#include <PGN/Utilities/SkeletalAnimation/Skeleton.h>
#include <PGN/Utilities/SkeletalAnimation/SkeletonFactory.h>
#include "SceneManager.h"

SceneManager::SceneManager() 
{
	assetStream = NULL;
	cacheStream = NULL;
	animStream = NULL;

	window = NULL;
	graphics = NULL;
	scene = NULL;
	camera = NULL;
	
	skelFactory = NULL;
	animFactory = NULL;
	
	pathFinderStream = NULL;
	pathFinder = NULL;
	pathFinderFactory = NULL;

	physicsStream = NULL;
	physicsFactory = NULL;
	physicsWorld = NULL;
	physicsWorldFactory = NULL;
}

SceneManager::~SceneManager() 
{
	destroy();
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

	physicsStream = pgn::createStdFileStream("");
	physicsFactory = pgn::PhysicsFactory::create(physicsStream);

	pathFinderStream = pgn::createStdFileStream("");
	pathFinderFactory = pgn::PathFinderFactory::create(pathFinderStream);
	pathFinder = pathFinderFactory->createPathFinder();

	physicsWorldFactory = pgn::PhysicsWorldFactory::create();
	physicsWorld = physicsWorldFactory->createPhysicsWorld();
	physicsWorld->init();

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
	for (auto& physics : physicsModels)
	{
		physics->destroy();
	}
	physicsWorld->destroy();
	pathFinder->destroy();
	pathFinderFactory->destroy();

	physicsFactory->destroy();
	physicsWorldFactory->destroy();
	
	animFactory->destroy();
	skelFactory->destroy();
	
	graphics->endDraw();
	graphics->destroy();

	pgn::destroyStdFileStream(pathFinderStream);
	pgn::destroyStdFileStream(physicsStream);
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

void SceneManager::setCamera(pgn::Float3* eye, pgn::Float3* lookAt)
{
	pgn::Float4x3 viewMat;
	pgn::Float3 up = { 0, 1, 0 };
	pgn::buildViewMat(eye, lookAt, &up, &viewMat);
	camera->setViewMat(&viewMat);
}

void SceneManager::screenPointToRay(int x, int y, pgn::Float3* origin, pgn::Float3* dir)
{
	camera->screenPointToRay(x, y, origin, dir);
}

void SceneManager::addGroundToPhysicsWorld(pgn::Physics* physics)
{
	physicsWorld->addGround(physics);
}

bool SceneManager::rayIntersect(pgn::Float3* rayBegin, pgn::Float3* rayDir, pgn::Float3* contactPos)
{
	return physicsWorld->rayIntersect(rayBegin, rayDir, contactPos);
}

float SceneManager::getGroundHeight(float x, float z)
{
	return physicsWorld->getGroundHeight(x, z);
}

void SceneManager::addPointLight(pgn::Float4 intensity, float radius, pgn::Float3* pos)
{
	pgn::PointLight* light = graphics->createPointLight();
	light->setIntensity(intensity[0], intensity[1], intensity[2], intensity[3]);
	light->setRadius(radius);
	pgn::ScenePointLight* sceneLight = scene->add(light);
	sceneLight->setPosition(pos);
	pointLights.push_back(light);
	scenePointLights.push_back(sceneLight);
}

pgn::Model* SceneManager::addModel(char* pnm, char* tex, pgn::Float3* pos, float scale)
{
	pgn::Model* model = graphics->createModel();
	model->setMesh(pnm);
	model->setDiffuseMap(0, tex);
	pgn::Entity* entity = graphics->createEntity();
	entity->setModel(model);

	pgn::SceneEntity* sceneEntity = scene->add(entity, false);
	sceneEntity->setScale(scale, scale);
	pgn::Float4x3 worldMat =
	{
		1, 0, 0, pos->v[0],
		0, 1, 0, pos->v[1],
		0, 0, 1, pos->v[2],
	};
	sceneEntity->setWorldMat(&worldMat);

	models.push_back(model);
	entities.push_back(entity);
	sceneEntities.push_back(sceneEntity);
	return model;
}

pgn::SceneEntity* SceneManager::addCharacter(char* pnm, char* tex, pgn::Float3* pos, float scale)
{
	pgn::Entity* entity = graphics->createEntity();
	pgn::Model* model = graphics->createModel();
	model->setMesh(pnm);
	model->setDiffuseMap(0, tex);
	entity->setModel(model);

	pgn::Skeleton* skel = skelFactory->createSkeleton();
	pgn::Animation* anim = animFactory->createAnimation();
	anim->set("res/idle1_f.pna");
	skel->playAnimation(anim);
	entity->setSkeleton(skel);
	pgn::SceneEntity* sceneEntity = scene->add(entity, false);
	sceneEntity->setScale(scale, scale);
	pgn::Float4x3 worldMat =
	{
		1, 0, 0, pos->v[0],
		0, 1, 0, pos->v[1],
		0, 0, 1, pos->v[2],
	};
	sceneEntity->setWorldMat(&worldMat);

	models.push_back(model);
	skeletons.push_back(skel);
	animations.push_back(anim);
	entities.push_back(entity);
	sceneEntities.push_back(sceneEntity);
	return sceneEntity;
}

pgn::Physics* SceneManager::addPhysics(char* phy)
{
	pgn::Physics* physics = physicsFactory->createPhysics();
	physics->set(phy);
	physicsModels.push_back(physics);
	return physics;
}

pgn::PathFinder* SceneManager::getPathFinder()
{
	return pathFinder;
}

