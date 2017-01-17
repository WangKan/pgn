#include <new>
#include <PGN/Utilities/Pool.h>
#include <PGN/Utilities/SkeletalAnimation/SkeletonTemplateFactory.h>
#include "../EditableGeometry.h"
#include "EditablePNM.h"
#include "EditablePNMFactory.h"

EditablePNMFactory::EditablePNMFactory(SubsetAllocator& subsetAllocator)
	: subsetAllocator(subsetAllocator)
{
	meshPool = pgn::Pool::create(sizeof(EditablePNM));
	geomPool = pgn::Pool::create(sizeof(EditableGeometry));
	skeletonTemplateFactory = pgn::SkeletonTemplateFactory::create();
}

EditablePNMFactory::~EditablePNMFactory()
{
	meshPool->destroy();
	geomPool->destroy();
	skeletonTemplateFactory->destroy();
}

pgn::Asset* EditablePNMFactory::create()
{
	EditableGeometry* geom = new(geomPool->alloc()) EditableGeometry(subsetAllocator);
	return new(meshPool->alloc()) EditablePNM(this, geom, skeletonTemplateFactory);
}

void EditablePNMFactory::recycle(pgn::Asset* asset)
{
	EditablePNM* pnm = (EditablePNM*)asset;
	EditableGeometry* geom = (EditableGeometry*)pnm->geom;

	pnm->~EditablePNM();
	meshPool->_free(asset);

	geom->~EditableGeometry();
	geomPool->_free(geom);
}
