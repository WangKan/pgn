#include <PGN/Utilities/ResourceManager/ResourceHandle.h>
#include <PGN/Utilities/ResourceManager/ResourceManager.h>
#include "Graphics.h"
#include "NavModel.h"
#include "Renderer/NavGeometry.h"

NavModel::NavModel(Graphics* graphics)
{
	this->graphics = graphics;
	_complete = false;
}

void NavModel::init()
{
	if (geomHandle)
		graphics->renderer.navGeomMgr->releaseResource(geomHandle);
}

void NavModel::dispose()
{
	init();
}

pgn::NavModel* Graphics::createNavModel()
{
	return new(navModelPool->alloc()) NavModel(this);
}

void NavModel::_free()
{
	graphics->pendingNavModelRemovals.push_back(this);
}

void NavModel::setMesh(const char fileName[])
{
	init();
	geomHandle = fileName ? graphics->renderer.navGeomMgr->getResource(fileName) : 0;
	_complete = false;
}

bool NavModel::complete()
{
	if (!_complete)
		_complete = geomHandle ? geomHandle->core() != 0 : false;

	return _complete;
}

void NavModel::getAabb(pgn::Float3* min, pgn::Float3* max)
{
	NavGeometry* geom = (NavGeometry*)geomHandle->core();
	*min = geom->aabb.min;
	*max = geom->aabb.max;
}
