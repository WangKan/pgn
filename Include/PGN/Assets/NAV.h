#pragma once
namespace pgn {

struct NAVHeader
{
	int numVertices;
	int numIndices;
	int verticesChunkOffset;
	int indicesChunkOffset;
	int adjacentNodesChunkOffset;
};

}
