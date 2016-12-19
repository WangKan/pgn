#include <new>
#include <PGN/Utilities/Pool.h>
#include <PGN/Utilities/SkeletalAnimation/SkeletonTemplateFactory.h>
#include "../EditableGeometry.h"
#include "EditableMesh.h"
#include "EditableMeshFactory.h"

EditableMeshFactory::EditableMeshFactory(SubsetAllocator& subsetAllocator)
	: subsetAllocator(subsetAllocator)
{
	meshPool = pgn::Pool::create(sizeof(EditableMesh));
	geomPool = pgn::Pool::create(sizeof(EditableGeometry));
	skeletonTemplateFactory = pgn::SkeletonTemplateFactory::create();
}

EditableMeshFactory::~EditableMeshFactory()
{
	meshPool->destroy();
	geomPool->destroy();
	skeletonTemplateFactory->destroy();
}

pgn::Asset* EditableMeshFactory::create()
{
	EditableGeometry* geom = new(geomPool->alloc()) EditableGeometry(subsetAllocator);
	return new(meshPool->alloc()) EditableMesh(this, geom, skeletonTemplateFactory);
}

void EditableMeshFactory::recycle(pgn::Asset* asset)
{
	EditableMesh* mesh = (EditableMesh*)asset;
	EditableGeometry* geom = (EditableGeometry*)mesh->geom;

	mesh->~EditableMesh();
	meshPool->_free(asset);

	geom->~EditableGeometry();
	geomPool->_free(geom);
}
