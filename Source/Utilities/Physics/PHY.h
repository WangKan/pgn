#define PGN_STATIC_IMPORT
#include <PGN/Utilities/ResourceManager/Asset.h>
#undef PGN_STATIC_IMPORT
#include "PhyMesh.h"

namespace pgn{
	class ManagedHeap;
	struct MemBlock;
}

class PHY : public pgn::Asset
{
	pgn::AssetFactory* factory;
	pgn::ManagedHeap* heap;

	int numVertices;
	int numIndices;
	pgn::MemBlock* vertices;
	pgn::MemBlock* indices;
	bool ready;

	PhyMesh phyMesh;

public:
	PHY(pgn::AssetFactory* factory, pgn::ManagedHeap* heap);
	virtual pgn::AssetFactory* getFactory();
	virtual bool cook(void* rawData);
	virtual bool submit(void* rawData, void* customArg);
	virtual void unload(void* customArg);
	virtual void* core();
};
