#include <PGN/Assets/NAV.h>
#include <PGN/Utilities/ManagedHeap.h>
#include <string.h>
#include "NAV.h"

NAV::NAV(pgn::AssetFactory* factory, pgn::ManagedHeap* heap)
{
	this->factory = factory;
	this->heap = heap;
	ready = false;
}

pgn::AssetFactory* NAV::getFactory()
{
	return factory;
}

bool NAV::cook(void* rawData)
{
	pgn::NAVHeader* header = (pgn::NAVHeader*)rawData;

	numVertices = header->numVertices;
	numIndices = header->numIndices;

	int size;

	size = 12 * numVertices;
	vertices = heap->alloc(size);
	memcpy(vertices->p, (char*)header + header->verticesChunkOffset, size);

	size = 12 * numIndices;
	indices = heap->alloc(2 * numIndices);
	memcpy(indices->p, (char*)header + header->indicesChunkOffset, size);

	size = 2 * numIndices / 3 * 3; // todo: 正确的个数？
	adjacentNodes = heap->alloc(size);
	memcpy(adjacentNodes->p, (char*)header + header->adjacentNodesChunkOffset, size);

	ready = true;
	return true;
}

bool NAV::submit(void* rawData, void* customArg)
{
	return true;
}

void NAV::unload(void* customArg)
{
	if (!ready)
		return;

	heap->_free(vertices);
	heap->_free(indices);
	heap->_free(adjacentNodes);
}

void* NAV::core()
{
	if (!ready) return 0;

	navMesh.numVertices = numVertices;
	navMesh.numIndices = numIndices;
	navMesh.vertexBuffer = vertices->p;
	navMesh.indexBuffer = indices->p;
	navMesh.adjacentNodes = adjacentNodes->p;

	return &navMesh;
}
