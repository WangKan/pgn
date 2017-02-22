#define PGN_DLL_EXPORT
#include <PGN/Graphics/SceneManager.h>
#undef PGN_DLL_EXPORT

#define PGN_STATIC_IMPORT
#include <PGN/Common/debug_new.h>
#include <PGN/FileStream/StdFileStream.h>
#include <PGN/Graphics/Camera.h>
#include <PGN/Graphics/SkeletalModel.h>
#include <PGN/Graphics/Graphics.h>
#include <PGN/Graphics/Model.h>
#include <PGN/Graphics/NavModel.h>
#include <PGN/Graphics/PointLight.h>
#include <PGN/Graphics/Scene.h>
#include <PGN/Graphics/SceneEntity.h>
#include <PGN/Graphics/ScenePointLight.h>
#include <PGN/Math/Math.h>
#include <PGN/Platform/UI/Window.h>
#include <PGN/Utilities/LinearTransformations.h>
#include <PGN/Utilities/Physics/Physics.h>
#include <PGN/Utilities/Physics/PhysicsFactory.h>
#include <PGN/Utilities/Physics/PhysicsWorld.h>
#include <PGN/Utilities/Physics/PhysicsWorldFactory.h>
#include <PGN/Utilities/SkeletalAnimation/Animation.h>
#include <PGN/Utilities/SkeletalAnimation/AnimationFactory.h>
#include <PGN/Utilities/SkeletalAnimation/Skeleton.h>
#include <PGN/Utilities/SkeletalAnimation/SkeletonFactory.h>
#undef PGN_STATIC_IMPORT

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

	physicsStream = NULL;
	physicsFactory = NULL;
	physicsWorld = NULL;
	physicsWorldFactory = NULL;
}

SceneManager::~SceneManager() 
{
}

void SceneManager::dispose()
{
}

void SceneManager::_free()
{
	for (auto& light : scenePointLights)
	{
		scene->remove(light);
	}
	for (auto& entity : sceneModels)
	{
		scene->removeModel(entity.second);
	}
	for (auto& entity : sceneSkeletalModels)
	{
		scene->removeSkeletalModel(entity.second);
	}
	for (auto& entity : sceneNavModels)
	{
		scene->removeNavModel(entity);
	}

	scene->destroy();
	camera->destroy();

	for (auto& light : pointLights)
	{
		light->destroy();
	}
	for (auto& entity : skeletalModels)
	{
		entity->destroy();
	}
	for (auto& model : models)
	{
		model->destroy();
	}
	for (auto& model : navModels)
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

	physicsFactory->destroy();
	physicsWorldFactory->destroy();

	animFactory->destroy();
	skelFactory->destroy();

	graphics->endDraw();
	graphics->destroy();

	pgn::destroyStdFileStream(physicsStream);
	pgn::destroyStdFileStream(animStream);
	pgn::destroyStdFileStream(assetStream);
	pgn::destroyStdFileStream(cacheStream);

	delete this;
}

void SceneManager::init(pgn::Window* wnd) 
{
	window = wnd;
	assetStream = pgn::createStdFileStream("");
	cacheStream = pgn::createStdFileStream("");
	graphics = pgn::Graphics::create(window->getDisplay(), assetStream, cacheStream);
	scene = graphics->createScene();
	camera = graphics->createCamera();

	animStream = pgn::createStdFileStream("");
	skelFactory = pgn::SkeletonFactory::create();
	animFactory = pgn::AnimationFactory::create(animStream);

	physicsStream = pgn::createStdFileStream("");
	physicsFactory = pgn::PhysicsFactory::create(physicsStream);

	physicsWorldFactory = pgn::PhysicsWorldFactory::create();
	physicsWorld = physicsWorldFactory->createPhysicsWorld();
	physicsWorld->init();

	graphics->beginDraw(window);
}

void SceneManager::tick(int dt)
{
	for (auto& skeleton : skeletons)
	{
		skeleton->updatePose(dt);
	}
	
	float aspectRatio = (float)window->getClientWidth() / (float)window->getClientHeight();
	camera->setFrustumFovLH(60.0f / 180.0f*3.14f, aspectRatio, 0.01f, 1024);
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

void SceneManager::setViewMat(pgn::Float4x3* viewMat)
{
	camera->setViewMat(viewMat);
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

void SceneManager::addPointLight(pgn::Float4* intensity, float radius, pgn::Float3* pos)
{
	pgn::PointLight* light = graphics->createPointLight();
	light->setIntensity(intensity->v[0], intensity->v[1], intensity->v[2], intensity->v[3]);
	light->setRadius(radius);
	pgn::ScenePointLight* sceneLight = scene->add(light);
	sceneLight->setPosition(pos);
	pointLights.push_back(light);
	scenePointLights.push_back(sceneLight);
}

pgn::NavModel* SceneManager::addNavModel(char* nav, pgn::Float3* pos, float scale)
{
	pgn::NavModel* model = graphics->createNavModel();
	model->setMesh(nav);

	pgn::SceneEntity* sceneEntity = scene->addNavModel(model, false);
	sceneEntity->setScale(scale, scale);
	pgn::Float4x3 worldMat =
	{
		1, 0, 0, pos->x,
		0, 1, 0, pos->y,
		0, 0, 1, pos->z,
	};
	sceneEntity->setWorldMat(&worldMat);

	navModels.push_back(model);
	sceneNavModels.push_back(sceneEntity);
	return model;
}

pgn::SceneEntity* SceneManager::addModel(char* name, char* pnm, char* tex, pgn::Float3* pos, float scale)
{
	pgn::Model* model = graphics->createModel();
	model->setMesh(pnm);
	model->setDiffuseMap(0, tex);

	pgn::SceneEntity* sceneEntity = scene->addModel(model, false);
	sceneEntity->setScale(scale, scale);
	pgn::Float4x3 worldMat =
	{
		1, 0, 0, pos->x,
		0, 1, 0, pos->y,
		0, 0, 1, pos->z,
	};
	sceneEntity->setWorldMat(&worldMat);

	models.push_back(model);
	sceneModels[name] = sceneEntity;
	return sceneEntity;
}

pgn::SceneEntity* SceneManager::addCharacter(char* name, char* mesh, char* tex, pgn::Float3* pos, float scale)
{
	pgn::SkeletalModel* skeletalModel = graphics->createSkeletalModel();
	pgn::Model* model = graphics->createModel();
	model->setMesh(mesh);
	model->setDiffuseMap(0, tex);
	skeletalModel->setModel(model);

	pgn::Skeleton* skel = skelFactory->createSkeleton();
	pgn::Animation* anim = animFactory->createAnimation();
	anim->set("res/idle1_f.pna");
	skel->playAnimation(anim);
	skeletalModel->setSkeleton(skel);
	pgn::SceneEntity* sceneEntity = scene->addSkeletalModel(skeletalModel, false);
	sceneEntity->setScale(scale, scale);
	pgn::Float4x3 worldMat =
	{
		1, 0, 0, pos->x,
		0, 1, 0, pos->y,
		0, 0, 1, pos->z,
	};
	sceneEntity->setWorldMat(&worldMat);

	models.push_back(model);
	skeletons.push_back(skel);
	animations.push_back(anim);
	skeletalModels.push_back(skeletalModel);
	sceneSkeletalModels[name] = sceneEntity;
	return sceneEntity;
}

pgn::SceneEntity* SceneManager::getCharacter(char* name)
{
	std::map<std::string, pgn::SceneEntity*>::iterator it = sceneSkeletalModels.find(name);
	if (it == sceneSkeletalModels.end())
		return NULL;
	return it->second;
}

void SceneManager::delCharacter(char* name)
{
	pgn::SceneEntity* cha = getCharacter(name);
	if (cha != NULL)
		scene->removeSkeletalModel(cha);
}

void SceneManager::clear()
{
	for (auto& entity : sceneSkeletalModels)
	{
		scene->removeSkeletalModel(entity.second);
	}

	sceneSkeletalModels.clear();
}

pgn::Physics* SceneManager::addPhysics(char* phy)
{
	pgn::Physics* physics = physicsFactory->createPhysics();
	physics->set(phy);
	physicsModels.push_back(physics);
	return physics;
}

pgn::SceneManager* pgn::SceneManager::create()
{
	return debug_new ::SceneManager;
}

