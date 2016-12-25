#include <PGN/Utilities/PathFinding/PathFinder.h>

namespace pgn {
	class ResourceHandle;
	class ResourceManager;
	class Pool;
}

class PathFinder : public pgn::PathFinder
{
public:
	pgn::Pool* pool;
	pgn::ResourceManager* navMeshMgr;
	pgn::ResourceHandle* navMesh;
	bool _ready;

	PathFinder(pgn::Pool* pool, pgn::ResourceManager* navMeshMgr);
	void init();
	virtual void dispose();
	virtual void _free();
	virtual void setNavigationMesh(char fileName[]);
	virtual bool ready();
};
