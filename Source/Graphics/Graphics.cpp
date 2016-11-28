#include <PGN/Assets/PNM.h>
#include <PGN/Common/debug_new.h>
#include <PGN/Utilities/Heap.h>
#include "DirectionalLight.h"
#include "Entity.h"
#include "Graphics.h"
#include "NavModel.h"
#include "Model.h"
#include "PointLight.h"

Graphics::Graphics(pgn::Display displayPrototype, pgn::FileStream* assetStream, pgn::FileStream* cacheStream)
	: renderer(displayPrototype, assetStream, cacheStream)
{
	navModelPool = pgn::Pool::create(sizeof(NavModel));
	modelPool = pgn::Pool::create(sizeof(Model));
	entityPool = pgn::Pool::create(sizeof(Entity));
	pointLightPool = pgn::Pool::create(sizeof(PointLight));
	dirLightPool = pgn::Pool::create(sizeof(DirectionalLight));
	tmpBuf = pgn::Heap::create();
}

void Graphics::dispose()
{
	navModelPool->destroy();
	modelPool->destroy();
	entityPool->destroy();
	pointLightPool->destroy();
	dirLightPool->destroy();
	tmpBuf->destroy();
}

pgn::Graphics* pgn::Graphics::create(pgn::Display displayPrototype, pgn::FileStream* assetStream, pgn::FileStream* cacheStream)
{
	return debug_new ::Graphics(displayPrototype, assetStream, cacheStream);
}

void Graphics::_free()
{
	delete this;
}

void Graphics::beginDraw(pgn::Window* wnd)
{
	//renderer.beginDraw(wnd, &lightIndexedDeferredRendering);
	renderer.beginDraw(wnd, &lightIndexedForwardRendering);
}

void Graphics::endDraw()
{
	renderer.finish();
	performPendingRemovals();
	renderer.endDraw();
}

void Graphics::performPendingRemovals()
{
	while (!pendingRemovals.empty())
	{
		Model* model = pendingRemovals.front();

		if (model->submittingStamp > renderer.finishCount) break;

		model->~Model();
		modelPool->_free(model);
		pendingRemovals.pop_front();
	}
}
