#include <list>
#include <PGN/Graphics/Scene.h>
#include <vector>
#include "SceneDirectionalLight.h"
#include "SceneEntity.h"
#include "ScenePointLight.h"

class Scene : public pgn::Scene
{
public:
	SceneEntity::StdList sceneModels;
	SceneEntity::StdList sceneSkeletalModels;
	SceneEntity::StdList sceneNavModels;
	ScenePointLight::StdList scenePointLights;
	SceneDirectionalLight::StdList sceneDirLights;

	static const int numLightDisjointSets = 4;
	std::vector<ScenePointLight*> lightDisjointSets[numLightDisjointSets];

	Scene();
	virtual void dispose();
	virtual void _free();

	virtual pgn::SceneEntity* addModel(pgn::Model* model, bool useInstancedDrawing);
	virtual void removeModel(pgn::SceneEntity* sceneEntity);

	virtual pgn::SceneEntity* addSkeletalModel(pgn::SkeletalModel* model, bool useInstancedDrawing);
	virtual void removeSkeletalModel(pgn::SceneEntity* sceneEntity);

	virtual pgn::SceneEntity* addNavModel(pgn::NavModel* model, bool useInstancedDrawing);
	virtual void removeNavModel(pgn::SceneEntity* sceneEntity);

	virtual pgn::ScenePointLight* add(pgn::PointLight* light);
	virtual void remove(pgn::ScenePointLight* scenePointLight);

	virtual pgn::SceneDirectionalLight* add(pgn::DirectionalLight* light);
	virtual void remove(pgn::SceneDirectionalLight* sceneDirLight);
};
