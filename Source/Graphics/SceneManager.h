#pragma once
#include <PGN/Graphics/SceneManager.h>
#include <list>
#include <map>
#include <string>

namespace pgn{
	struct Float3;
	struct Float4;
	struct Float4x3;
	class Graphics;
	class Scene;
	class Camera;
	class Model;
	class NavModel;
	class SkeletalModel;
	class PointLight;
	class SceneEntity;
	class Skeleton;
	class Animation;
	class FileStream;
	class SkeletonFactory;
	class AnimationFactory;
	class Physics;
	class PhysicsFactory;
	class PhysicsWorld;
	class PhysicsWorldFactory;
	class ScenePointLight;
}

class SceneManager : public pgn::SceneManager
{
public:
	SceneManager();
	virtual ~SceneManager();
	virtual void init(pgn::Window* wnd);
	virtual void tick(int dt);
	virtual void dispose();
	virtual void _free();
	virtual void clear();

	virtual void setCamera(pgn::Float3* eye, pgn::Float3* lookAt);
	virtual void setViewMat(pgn::Float4x3* viewMat);
	virtual void screenPointToRay(int x, int y, pgn::Float3* origin, pgn::Float3* dir);
	virtual void addGroundToPhysicsWorld(pgn::Physics* physics);
	virtual bool rayIntersect(pgn::Float3* rayBegin, pgn::Float3* rayDir, pgn::Float3* contactPos);
	virtual float getGroundHeight(float x, float z);

	virtual void addPointLight(pgn::Float4* intensity, float radius, pgn::Float3* pos);
	virtual pgn::NavModel* addNavModel(char* nav, pgn::Float3* pos, float scale);
	virtual pgn::SceneEntity* addModel(char* name, char* pnm, char* tex, pgn::Float3* pos, float scale);
	virtual pgn::SceneEntity* addCharacter(char* name, char* mesh, char* tex, pgn::Float3* pos, float scale);
	virtual pgn::SceneEntity* getCharacter(char* name);
	virtual void delCharacter(char* name);
	virtual pgn::Physics* addPhysics(char* phy);

private:
	std::map<std::string, pgn::SceneEntity* > sceneModels;
	std::map<std::string, pgn::SceneEntity* > sceneSkeletalModels;
	std::list<pgn::SceneEntity* > sceneNavModels;
	std::list<pgn::ScenePointLight* > scenePointLights;
	std::list<pgn::Model* > models;
	std::list<pgn::NavModel* > navModels;
	std::list<pgn::Skeleton* > skeletons;
	std::list<pgn::Animation* > animations;
	std::list<pgn::SkeletalModel* > skeletalModels;
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

	pgn::FileStream* physicsStream;
	pgn::PhysicsFactory* physicsFactory;
	pgn::PhysicsWorld* physicsWorld;
	pgn::PhysicsWorldFactory* physicsWorldFactory;
};
