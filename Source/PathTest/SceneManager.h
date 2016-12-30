#pragma once
#include <list>

namespace pgn{
	struct Float3;
	struct Float4;
	struct Float4x3;
	class Graphics;
	class Scene;
	class Camera;
	class Model;
	class Entity;
	class PointLight;
	class SceneEntity;
	class Skeleton;
	class Animation;
	class FileStream;
	class SkeletonFactory;
	class AnimationFactory;
	class PathFinder;
	class PathFinderFactory;
	class Physics;
	class PhysicsFactory;
	class PhysicsWorld;
	class PhysicsWorldFactory;
	class ScenePointLight;
}

class SceneManager
{
public:
	SceneManager();
	virtual ~SceneManager();
	void init(pgn::Window* wnd);
	void destroy();
	void tick(int dt);
	
	void setCamera(pgn::Float3* eye, pgn::Float3* lookAt);
	void screenPointToRay(int x, int y, pgn::Float3* origin, pgn::Float3* dir);
	void addGroundToPhysicsWorld(pgn::Physics* physics);
	bool rayIntersect(pgn::Float3* rayBegin, pgn::Float3* rayDir, pgn::Float3* contactPos);
	float getGroundHeight(float x, float z);

	void addPointLight(pgn::Float4 intensity, float radius, pgn::Float3* pos);
	pgn::Model* addModel(char* pnm, char* tex, pgn::Float3* pos, float scale);
	pgn::SceneEntity* addCharacter(char* pnm, char* tex, pgn::Float3* pos, float scale);
	pgn::Physics* addPhysics(char* phy);
	pgn::PathFinder* getPathFinder();

private:
	std::list<pgn::SceneEntity* > sceneEntities;
	std::list<pgn::ScenePointLight* > scenePointLights;
	std::list<pgn::Model* > models;
	std::list<pgn::Skeleton* > skeletons;
	std::list<pgn::Animation* > animations;
	std::list<pgn::Entity* > entities;
	std::list<pgn::PointLight* > pointLights;
	std::list<pgn::Physics* > physicsModels;
	
	pgn::FileStream* assetStream;
	pgn::FileStream* cacheStream;
	pgn::FileStream* animStream;

	pgn::Window* window;
	pgn::Graphics* graphics;
	pgn::Scene* scene;
	pgn::Camera* camera;
	pgn::SkeletonFactory* skelFactory;
	pgn::AnimationFactory* animFactory;

	pgn::FileStream* pathFinderStream;
	pgn::PathFinder* pathFinder;
	pgn::PathFinderFactory* pathFinderFactory;

	pgn::FileStream* physicsStream;
	pgn::PhysicsFactory* physicsFactory;
	pgn::PhysicsWorld* physicsWorld;
	pgn::PhysicsWorldFactory* physicsWorldFactory;
};
