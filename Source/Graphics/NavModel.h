#include <PGN/Graphics/NavModel.h>

namespace pgn {
	class FileStream;
}
class Graphics;

class NavModel : public pgn::NavModel
{
public:
	Graphics* graphics;
	pgn::FileStream* fileStream;
	char* buf;
	int numVertices;
	int numIndices;
	void* vertexBuffer;
	void* indexBuffer;
	void* adjacentNodes;

	NavModel(Graphics* graphics);
	virtual ~NavModel();
	virtual void dispose();
	virtual void _free();
	virtual void setMesh(char fileName[]);
	virtual void* getVertexBuffer()  { return vertexBuffer; }
	virtual void* getIndexBuffer()   { return indexBuffer; }
	virtual void* getAdjacentNodes() { return adjacentNodes; }
	virtual int getVertexCount()     { return numVertices; }
	virtual int getIndexCount()		 { return numIndices; }
};
