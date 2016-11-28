#pragma once
#include "../Common/DllInterface.h"
#include "../Common/Interface.h"
#include <PGN/Platform/Graphics/Display.h>
namespace pgn {

class Camera;
class DirectionalLight;
class Entity;
class FileStream;
class NavModel;
class Model;
class PointLight;
class Scene;
class Window;

class Graphics : public Interface
{
public:
	static PGN_DLL_INTERFACE Graphics* create(Display displayPrototype, FileStream* assetStream, FileStream* cacheStream);

	virtual Scene* createScene() = 0;
	virtual Camera* createCamera() = 0;
	virtual Model* createModel() = 0;
	virtual NavModel* createNavModel() = 0;
	virtual Entity* createEntity() = 0;
	virtual PointLight* createPointLight() = 0;
	virtual DirectionalLight* createDirectionalLight() = 0;
	virtual void beginDraw(pgn::Window* wnd) = 0;
	virtual void endDraw() = 0;
};

}
