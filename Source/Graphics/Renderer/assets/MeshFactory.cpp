#include <new>
#include <PGN/Utilities/Pool.h>
#include <PGN/Utilities/SkeletalAnimation/SkeletonTemplateFactory.h>
#include "../Geometry.h"
#include "Mesh.h"
#include "MeshFactory.h"

MeshFactory::MeshFactory(SubsetAllocator& subsetAllocator)
	: subsetAllocator(subsetAllocator)
{
	meshPool = pgn::Pool::create(sizeof(Mesh));
	geomPool = pgn::Pool::create(sizeof(Geometry));
	skeletonTemplateFactory = pgn::SkeletonTemplateFactory::create();
}

MeshFactory::~MeshFactory()
{
	meshPool->destroy();
	geomPool->destroy();
	skeletonTemplateFactory->destroy();
}

pgn::Asset* MeshFactory::create()
{
	Geometry* geom = new(geomPool->alloc()) Geometry(subsetAllocator);
	return new(meshPool->alloc()) Mesh(this, geom, skeletonTemplateFactory);
}

void MeshFactory::recycle(pgn::Asset* asset)
{
	Mesh* mesh = (Mesh*)asset;
	Geometry* geom = mesh->geom;

	mesh->~Mesh();
	meshPool->_free(asset);

	geom->~Geometry();
	geomPool->_free(geom);
}
