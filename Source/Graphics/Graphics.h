#define PGN_DLL_EXPORT
#include <PGN/Graphics/Graphics.h>
#undef PGN_DLL_EXPORT

#include "Renderer/Renderer.h"
#include "Renderer/TextureInfo.h"

class EditableModel;
class Model;
class NavModel;

class Graphics : public pgn::Graphics
{
public:
	Renderer renderer;
	pgn::Pool* modelPool;
	pgn::Pool* editableModelPool;
	TextureSetAllocator texSetAllocator;
	pgn::Pool* skeletalModelPool;
	pgn::Pool* navModelPool;
	pgn::Pool* pointLightPool;
	pgn::Pool* dirLightPool;
	pgn::Heap* tmpBuf;
	std::list<Model*, pgn::PoolAllocator<Model*, pgn::Pool>> pendingModelRemovals;
	std::list<EditableModel*, pgn::PoolAllocator<EditableModel*, pgn::Pool>> pendingEditableModelRemovals;
	std::list<NavModel*, pgn::PoolAllocator<NavModel*, pgn::Pool>> pendingNavModelRemovals;

	Graphics(pgn::Display displayPrototype, pgn::FileStream* assetStream, pgn::FileStream* cacheStream);
	virtual void dispose();
	virtual void _free();
	virtual void					beginDraw(pgn::Window* wnd);
	virtual pgn::Camera*			createCamera();
	virtual pgn::DirectionalLight*	createDirectionalLight();
	virtual pgn::EditableModel*		createEditableModel();
	virtual pgn::Model*				createModel();
	virtual pgn::NavModel*			createNavModel();
	virtual pgn::PointLight*		createPointLight();
	virtual pgn::Scene*				createScene();
	virtual pgn::SkeletalModel*		createSkeletalModel();
	virtual void					endDraw();
	void performPendingRemovals();
};
