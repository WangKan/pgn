#pragma once
#include "../Common/DllInterface.h"
#include "../Common/Interface.h"

namespace pgn {

struct Float3;
struct Float4;
struct Float4x3;
class Model;
class NavModel;
class Physics;
class SceneEntity;
class Window;

class SceneManager : public Interface
{
public:
	static PGN_DLL_INTERFACE SceneManager* create();
	virtual void init(Window* wnd) = 0;
	virtual void tick(int dt) = 0;
	virtual void clear() = 0;
	
	virtual void setCamera(Float3* eye, Float3* lookAt) = 0;
	virtual void setViewMat(Float4x3* viewMat) = 0;
	virtual void screenPointToRay(int x, int y, Float3* origin, Float3* dir) = 0;
	virtual void addGroundToPhysicsWorld(Physics* physics) = 0;
	virtual bool rayIntersect(Float3* rayBegin, Float3* rayDir, Float3* contactPos) = 0;
	virtual float getGroundHeight(float x, float z) = 0;

	virtual void addPointLight(Float4* intensity, float radius, Float3* pos) = 0;
	virtual NavModel* addNavModel(char* nav, pgn::Float3* pos, float scale) = 0;
	virtual SceneEntity* addModel(char* name, char* pnm, char* tex, pgn::Float3* pos, float scale) = 0;
	virtual SceneEntity* addCharacter(char* name, char* mesh, char* tex, Float3* pos, float scale) = 0;
	virtual void delCharacter(char* name) = 0;
	virtual SceneEntity* getCharacter(char* name) = 0;
	virtual Physics* addPhysics(char* phy) = 0;
};

}
