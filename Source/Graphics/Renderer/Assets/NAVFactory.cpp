#include <new>
#include <PGN/Utilities/Pool.h>
#include "../NavGeometry.h"
#include "NAV.h"
#include "NAVFactory.h"

NAVFactory::NAVFactory(SubsetAllocator& subsetAllocator)
	: subsetAllocator(subsetAllocator)
{
	meshPool = pgn::Pool::create(sizeof(NAV));
	geomPool = pgn::Pool::create(sizeof(NavGeometry));
}

NAVFactory::~NAVFactory()
{
	meshPool->destroy();
	geomPool->destroy();
}

pgn::Asset* NAVFactory::create()
{
	NavGeometry* geom = new(geomPool->alloc()) NavGeometry(subsetAllocator);
	return new(meshPool->alloc()) NAV(this, geom);
}

void NAVFactory::recycle(pgn::Asset* asset)
{
	NAV* mesh = (NAV*)asset;
	NavGeometry* geom = mesh->geom;

	mesh->~NAV();
	meshPool->_free(asset);

	geom->~NavGeometry();
	geomPool->_free(geom);
}
