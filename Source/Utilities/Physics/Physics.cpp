#define PGN_STATIC_IMPORT
#include <PGN/Utilities/ResourceManager/ResourceHandle.h>
#include <PGN/Utilities/ResourceManager/ResourceManager.h>
#undef PGN_STATIC_IMPORT
#include "Physics.h"
#include "PhyMesh.h"

Physics::Physics(pgn::Pool* pool, pgn::ResourceManager* physicsMgr)
{
	this->pool = pool;
	this->physicsMgr = physicsMgr;
	physics = 0;
	_complete = false;
}

void Physics::init()
{
	if (physics) physicsMgr->releaseResource(physics);
}

void Physics::dispose()
{
	init();
}

void Physics::set(char fileName[])
{
	init();
	physics = fileName ? physicsMgr->getResource(fileName) : 0;
	_complete = false;
}

bool Physics::complete()
{
	if (!_complete)
		_complete = physics->core() != 0;

	return _complete;
}

