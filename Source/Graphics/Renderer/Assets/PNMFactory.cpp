#include <new>
#include <PGN/Utilities/Pool.h>
#include <PGN/Utilities/SkeletalAnimation/SkeletonTemplateFactory.h>
#include "../Geometry.h"
#include "PNM.h"
#include "PNMFactory.h"

PNMFactory::PNMFactory(SubsetAllocator& subsetAllocator)
	: subsetAllocator(subsetAllocator)
{
	meshPool = pgn::Pool::create(sizeof(PNM));
	geomPool = pgn::Pool::create(sizeof(Geometry));
	skeletonTemplateFactory = pgn::SkeletonTemplateFactory::create();
}

PNMFactory::~PNMFactory()
{
	meshPool->destroy();
	geomPool->destroy();
	skeletonTemplateFactory->destroy();
}

pgn::Asset* PNMFactory::create()
{
	Geometry* geom = new(geomPool->alloc()) Geometry(subsetAllocator);
	return new(meshPool->alloc()) PNM(this, geom, skeletonTemplateFactory);
}

void PNMFactory::recycle(pgn::Asset* asset)
{
	PNM* pnm = (PNM*)asset;
	Geometry* geom = pnm->geom;

	pnm->~PNM();
	meshPool->_free(asset);

	geom->~Geometry();
	geomPool->_free(geom);
}
