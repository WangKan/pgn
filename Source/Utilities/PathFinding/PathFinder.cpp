#define PGN_STATIC_IMPORT
#include <PGN/Utilities/ResourceManager/ResourceHandle.h>
#include <PGN/Utilities/ResourceManager/ResourceManager.h>
#undef PGN_STATIC_IMPORT
#include "PathFinder.h"

PathFinder::PathFinder(pgn::Pool* pool, pgn::ResourceManager* navMeshMgr)
{
	this->pool = pool;
	this->navMeshMgr = navMeshMgr;
	navMesh = 0;
	_ready = false;
}

void PathFinder::init()
{
	if (navMesh) navMeshMgr->releaseResource(navMesh);
}

void PathFinder::dispose()
{
	init();
}

void PathFinder::setNavigationMesh(char fileName[])
{
	init();
	navMesh = fileName ? navMeshMgr->getResource(fileName) : 0;
	_ready = false;
}

bool PathFinder::ready()
{
	if (!_ready)
		_ready = navMesh->core() != 0;

	return _ready;
}
