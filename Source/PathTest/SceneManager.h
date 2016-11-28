#pragma once
#include <list>

namespace pgn{
	struct Float3;
	struct Float4;
	struct Float4x3;
	class Graphics;
	class Scene;
	class NavModel;
	class Model;
	class Entity;
	class PointLight;
	class SceneEntity;
	class ScenePointLight;
}

class SceneManager
{
public:
	SceneManager();
	virtual ~SceneManager();
	void init(pgn::Window* wnd);
	void destroy();
	void addPointLight(pgn::Float4 intensity, float radius, pgn::Float3 pos);
	pgn::NavModel* addNavModel(char* nav);
	pgn::Model* addModel(char* pnm, pgn::Float4x3& worldMat, float scale);
	pgn::SceneEntity* addCharacter(char* pnm, pgn::Float4x3& worldMat, float scale);
	void tick(int dt);
	void setCameraViewMat(pgn::Float4x3& viewMat);
	void screenPointToRay(pgn::Float3& begin, pgn::Float3& end, float x, float y);

private:
	std::list<pgn::SceneEntity* > sceneEntities;
	std::list<pgn::ScenePointLight* > scenePointLights;
	std::list<pgn::NavModel* > navModels;
	std::list<pgn::Model* > models;
	std::list<pgn::Skeleton* > skeletons;
	std::list<pgn::Animation* > animations;
	std::list<pgn::Entity* > entities;
	std::list<pgn::PointLight* > pointLights;
	
	pgn::Window* window;
	pgn::FileStream* assetStream;
	pgn::FileStream* cacheStream;
	pgn::Graphics* graphics;
	pgn::Scene* scene;
	pgn::Camera* camera;
	pgn::FileStream* animStream;
	pgn::SkeletonFactory* skelFactory;
	pgn::AnimationFactory* animFactory;
};
