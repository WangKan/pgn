#define PGN_DLL_EXPORT
#include <PGN/Utilities/Physics/PhysicsFactory.h>
#undef PGN_DLL_EXPORT
#include <new>
#include <PGN/Common/debug_new.h>
#define PGN_STATIC_IMPORT
#include <PGN/Utilities/Pool.h>
#include <PGN/Utilities/ResourceManager/AsyncLoader.h>
#include <PGN/Utilities/ResourceManager/ResourceManager.h>
#undef PGN_STATIC_IMPORT
#include "Physics.h"
#include "PHYFactory.h"

class PhysicsFactory : public pgn::PhysicsFactory
{
	pgn::Pool* pool;
	PHYFactory* phyFactory;
	pgn::ResourceManager* physicsMgr;
public:
	PhysicsFactory(pgn::FileStream* f);
	virtual void dispose();
	virtual void _free();
	virtual pgn::Physics* createPhysics();
};

PhysicsFactory::PhysicsFactory(pgn::FileStream* f)
{
	pool = pgn::Pool::create(sizeof(Physics));
	phyFactory = debug_new PHYFactory;
	physicsMgr = pgn::ResourceManager::create(phyFactory, f, pgn::getDefaultAsyncLoader());
}

void PhysicsFactory::dispose()
{
	physicsMgr->destroy();
//	delete phyFactory;
	pgn::releaseDefaultAsyncLoader();
	pool->destroy();
}

pgn::PhysicsFactory* pgn::PhysicsFactory::create(FileStream* f)
{
	return debug_new::PhysicsFactory(f);
}

void PhysicsFactory::_free()
{
	delete this;
}

pgn::Physics* PhysicsFactory::createPhysics()
{
	return new(pool->alloc()) Physics(pool, physicsMgr);
}

void Physics::_free()
{
	pool->_free(this);
}
