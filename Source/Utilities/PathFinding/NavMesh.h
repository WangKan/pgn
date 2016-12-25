
struct NavMesh
{
	int numVertices;
	int numIndices;
	void* vertexBuffer;
	void* indexBuffer;
	void* adjacentNodes;
};
