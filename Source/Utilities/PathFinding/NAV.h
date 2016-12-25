#define PGN_STATIC_IMPORT
#include <PGN/Utilities/ResourceManager/Asset.h>
#undef PGN_STATIC_IMPORT
#include "NavMesh.h"

namespace pgn{
	class ManagedHeap;
	struct MemBlock;
}

class NAV : public pgn::Asset
{
	pgn::AssetFactory* factory;
	pgn::ManagedHeap* heap;

	int numVertices;
	int numIndices;
	pgn::MemBlock* vertices;
	pgn::MemBlock* indices;
	pgn::MemBlock* adjacentNodes;
	bool ready;

	NavMesh navMesh;

public:
	NAV(pgn::AssetFactory* factory, pgn::ManagedHeap* heap);
	virtual pgn::AssetFactory* getFactory();
	virtual bool cook(void* rawData);
	virtual bool submit(void* rawData, void* customArg);
	virtual void unload(void* customArg);
	virtual void* core();
};
