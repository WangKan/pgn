#pragma once
namespace pgn {

struct PHYHeader
{
	int numVertices;
	int numIndices;
	int verticesChunkOffset;
	int indicesChunkOffset;
};

}
