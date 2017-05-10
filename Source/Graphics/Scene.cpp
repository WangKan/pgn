#include <PGN/Common/debug_new.h>
#include "Graphics.h"
#include "Model.h"
#include "NavModel.h"
#include "Scene.h"

Scene::Scene()
{
	for (auto& set : lightDisjointSets)
		set.reserve(SceneContext::maxNumPointLights);
}

void Scene::dispose()
{
}

pgn::Scene* Graphics::createScene()
{
	return debug_new Scene;
}

void Scene::_free()
{
	delete this;
}

pgn::SceneEntity* Scene::addModel(pgn::Model* model, bool useInstancedDrawing)
{
	sceneModels.emplace_front(dynamic_cast<Model*>(model), useInstancedDrawing);
	SceneEntity* sceneEntity = &sceneModels.front();
	sceneEntity->it = sceneModels.begin();
	return sceneEntity;
}

void Scene::removeModel(pgn::SceneEntity* _sceneEntity)
{
	SceneEntity* sceneEntity = (SceneEntity*)_sceneEntity;
	sceneModels.erase(sceneEntity->it);
}

pgn::SceneEntity* Scene::addSkeletalModel(pgn::SkeletalModel* model, bool useInstancedDrawing)
{
	sceneSkeletalModels.emplace_front((SkeletalModel*)model, useInstancedDrawing);
	SceneEntity* sceneEntity = &sceneSkeletalModels.front();
	sceneEntity->it = sceneSkeletalModels.begin();
	return sceneEntity;
}

void Scene::removeSkeletalModel(pgn::SceneEntity* _sceneEntity)
{
	SceneEntity* sceneEntity = (SceneEntity*)_sceneEntity;
	sceneSkeletalModels.erase(sceneEntity->it);
}

pgn::SceneEntity* Scene::addNavModel(pgn::NavModel* model, bool useInstancedDrawing)
{
	sceneNavModels.emplace_front(dynamic_cast<NavModel*>(model), useInstancedDrawing);
	SceneEntity* sceneEntity = &sceneNavModels.front();
	sceneEntity->it = sceneNavModels.begin();
	return sceneEntity;
}

void Scene::removeNavModel(pgn::SceneEntity* _sceneEntity)
{
	SceneEntity* sceneEntity = (SceneEntity*)_sceneEntity;
	sceneNavModels.erase(sceneEntity->it);
}

pgn::ScenePointLight* Scene::add(pgn::PointLight* light)
{
	scenePointLights.emplace_front((PointLight*)light);
	ScenePointLight* scenePointLight = &scenePointLights.front();
	scenePointLight->it = scenePointLights.begin();
	return scenePointLight;
}

void Scene::remove(pgn::ScenePointLight* _scenePointLight)
{
	ScenePointLight* scenePointLight = (ScenePointLight*)_scenePointLight;
	scenePointLights.erase(scenePointLight->it);
}

pgn::SceneDirectionalLight* Scene::add(pgn::DirectionalLight* light)
{
	sceneDirLights.emplace_front((DirectionalLight*)light);
	SceneDirectionalLight* sceneDirLight = &sceneDirLights.front();
	sceneDirLight->it = sceneDirLights.begin();
	return sceneDirLight;
}

void Scene::remove(pgn::SceneDirectionalLight* _sceneDirLight)
{
	SceneDirectionalLight* sceneDirLight = (SceneDirectionalLight*)_sceneDirLight;
	sceneDirLights.erase(sceneDirLight->it);
}
