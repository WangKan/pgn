#define PGN_DLL_EXPORT
#include <PGN/Graphics/Graphics.h>
#undef PGN_DLL_EXPORT

#include "Renderer/Renderer.h"
#include "Renderer/TextureInfo.h"

class NavModel;
class Model;

class Graphics : public pgn::Graphics
{
public:
	Renderer renderer;
	pgn::Pool* navModelPool;
	pgn::Pool* modelPool;
	TextureSetAllocator texSetAllocator;
	pgn::Pool* entityPool;
	pgn::Pool* pointLightPool;
	pgn::Pool* dirLightPool;
	pgn::Heap* tmpBuf;
	std::list<Model*, pgn::PoolAllocator<Model*, pgn::Pool>> pendingRemovals;

	Graphics(pgn::Display displayPrototype, pgn::FileStream* assetStream, pgn::FileStream* cacheStream);
	virtual void dispose();
	virtual void _free();
	virtual pgn::Scene* createScene();
	virtual pgn::Camera* createCamera();
	virtual pgn::Model* createModel();
	virtual pgn::NavModel* createNavModel();
	virtual pgn::Entity* createEntity();
	virtual pgn::PointLight* createPointLight();
	virtual pgn::DirectionalLight* createDirectionalLight();
	virtual void beginDraw(pgn::Window* wnd);
	virtual void endDraw();
	void performPendingRemovals();
};
