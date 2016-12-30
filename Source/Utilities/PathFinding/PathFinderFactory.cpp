#define PGN_DLL_EXPORT
#include <PGN/Utilities/PathFinding/PathFinderFactory.h>
#undef PGN_DLL_EXPORT

#include <new>
#include <PGN/Common/debug_new.h>
#define PGN_STATIC_IMPORT
#include <PGN/Utilities/Pool.h>
#include <PGN/Utilities/ResourceManager/AsyncLoader.h>
#include <PGN/Utilities/ResourceManager/ResourceManager.h>
#undef PGN_STATIC_IMPORT
#include "PathFinder.h"
#include "NAVFactory.h"

class PathFinderFactory : public pgn::PathFinderFactory
{
	pgn::Pool* pool;
	NAVFactory* navFactory;
	pgn::ResourceManager* navMeshMgr;
public:
	PathFinderFactory(pgn::FileStream* f);
	virtual void dispose();
	virtual void _free();
	virtual pgn::PathFinder* createPathFinder();
};

PathFinderFactory::PathFinderFactory(pgn::FileStream* f)
{
	pool = pgn::Pool::create(sizeof(PathFinder));
	navFactory = debug_new NAVFactory;
	navMeshMgr = pgn::ResourceManager::create(navFactory, f, pgn::getDefaultAsyncLoader());
}

void PathFinderFactory::dispose()
{
	navMeshMgr->destroy();
	//delete navMeshMgr;
	pgn::releaseDefaultAsyncLoader();
	pool->destroy();
}

pgn::PathFinderFactory* pgn::PathFinderFactory::create(FileStream* f)
{
	return debug_new ::PathFinderFactory(f);
}

void PathFinderFactory::_free()
{
	delete this;
}

pgn::PathFinder* PathFinderFactory::createPathFinder()
{
	return new(pool->alloc()) PathFinder(pool, navMeshMgr);
}

void PathFinder::_free()
{
	pool->_free(this);
}
