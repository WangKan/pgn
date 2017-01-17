#include <list>
#include <PGN/Graphics/Scene.h>
#include <vector>
#include "SceneDirectionalLight.h"
#include "SceneEntity.h"
#include "ScenePointLight.h"

class CBufAllocator;
class Graphics;
struct SceneEntityListItem;

class Scene : public pgn::Scene
{
public:
	Graphics* graphics;

	SceneEntity::StdList sceneEntities;
	ScenePointLight::StdList scenePointLights;
	SceneDirectionalLight::StdList sceneDirLights;

	static const int numLightDisjointSets = 4;
	std::vector<ScenePointLight*> lightDisjointSets[numLightDisjointSets];

	Scene(Graphics* graphics);
	virtual void dispose();
	virtual void _free();

	virtual pgn::SceneEntity* add(pgn::SkeletalModel* skeletalModel, bool useInstancedDrawing);
	virtual void remove(pgn::SceneEntity* sceneEntity);

	virtual pgn::ScenePointLight* add(pgn::PointLight* light);
	virtual void remove(pgn::ScenePointLight* scenePointLight);

	virtual pgn::SceneDirectionalLight* add(pgn::DirectionalLight* light);
	virtual void remove(pgn::SceneDirectionalLight* sceneDirLight);

	virtual void commit(pgn::Camera* camera);

	void submitEntities(SceneEntityListItem* first, int count, CBufAllocator* cbufAllocator);
};
