#define PGN_DLL_EXPORT
#include <PGN/Utilities/Physics/PhysicsWorldFactory.h>
#undef PGN_DLL_EXPORT

#include <new>
#include <PGN/Common/debug_new.h>
#define PGN_STATIC_IMPORT
#include <PGN/Utilities/Pool.h>
#undef PGN_STATIC_IMPORT
#include "PhysicsWorld.h"

class PhysicsWorldFactory : public pgn::PhysicsWorldFactory
{
	pgn::Pool* pool;
public:
	PhysicsWorldFactory();
	virtual void dispose();
	virtual void _free();
	virtual pgn::PhysicsWorld* createPhysicsWorld();
};

PhysicsWorldFactory::PhysicsWorldFactory()
{
	pool = pgn::Pool::create(sizeof(PhysicsWorld));
}

void PhysicsWorldFactory::dispose()
{
	pool->destroy();
}

pgn::PhysicsWorldFactory* pgn::PhysicsWorldFactory::create()
{
	return debug_new ::PhysicsWorldFactory;
}

void PhysicsWorldFactory::_free()
{
	delete this;
}

pgn::PhysicsWorld* PhysicsWorldFactory::createPhysicsWorld()
{
	return new(pool->alloc()) PhysicsWorld(pool);
}

