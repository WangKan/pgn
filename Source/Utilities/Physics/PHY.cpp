#include <PGN/Assets/PHY.h>
#include <PGN/Utilities/ManagedHeap.h>
#include <string.h>
#include "PHY.h"

PHY::PHY(pgn::AssetFactory* factory, pgn::ManagedHeap* heap)
{
	this->factory = factory;
	this->heap = heap;
	ready = false;
}

pgn::AssetFactory* PHY::getFactory()
{
	return factory;
}

bool PHY::cook(void* rawData)
{
	pgn::PHYHeader* header = (pgn::PHYHeader*)rawData;

	numVertices = header->numVertices;
	numIndices = header->numIndices;

	int size;

	size = 12 * numVertices;
	vertices = heap->alloc(size);
	memcpy(vertices->p, (char*)header + header->verticesChunkOffset, size);

	size = 12 * numIndices;
	indices = heap->alloc(2 * numIndices);
	memcpy(indices->p, (char*)header + header->indicesChunkOffset, size);

	ready = true;
	return true;
}

bool PHY::submit(void* rawData, void* customArg)
{
	return true;
}

void PHY::unload(void* customArg)
{
	if (!ready)
		return;

	heap->_free(vertices);
	heap->_free(indices);
}

void* PHY::core()
{
	if (!ready) return 0;

	phyMesh.numVertices = numVertices;
	phyMesh.numIndices = numIndices;
	phyMesh.vertexBuffer = vertices->p;
	phyMesh.indexBuffer = indices->p;

	return &phyMesh;
}
