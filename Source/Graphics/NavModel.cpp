#include <PGN/Assets/NAV.h>
#include <PGN/Common/debug_new.h>
#include <PGN/FileStream/FileStream.h>
#include "Graphics.h"
#include "NavModel.h"

NavModel::NavModel(Graphics* graphics)
{
	this->graphics = graphics;
	this->fileStream = graphics->renderer.cacheStream;
}

NavModel::~NavModel()
{
	if (buf != NULL)
		delete[] buf;
}

pgn::NavModel* Graphics::createNavModel()
{
	return new(navModelPool->alloc()) NavModel(this);
}

void NavModel::dispose()
{
}

void NavModel::_free()
{
	graphics->navModelPool->_free(this);
}

void NavModel::setMesh(char fileName[])
{
	if (fileStream == NULL)
		return;
	fileStream->open(fileName, pgn::FileStream::in);
	if (fileStream->isOpen())
	{
		long long bufSize = fileStream->getSize();
		buf = debug_new char[bufSize];
		fileStream->read(buf, bufSize);

		pgn::NAVHeader* header = (pgn::NAVHeader*)buf;
		numVertices = header->numVertices;
		numIndices = header->numIndices;
		vertexBuffer = buf + header->verticesChunkOffset;
		indexBuffer = buf + header->indicesChunkOffset;
		adjacentNodes = buf + header->adjacentNodesChunkOffset;

		fileStream->close();
	}
}


