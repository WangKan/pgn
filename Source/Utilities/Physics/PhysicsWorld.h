#pragma once
#include <PGN/Utilities/Physics/PhysicsWorld.h>
#include <ode/ode.h>

namespace pgn {
	class Pool;
}

class PhysicsWorld : public pgn::PhysicsWorld
{
public:
	PhysicsWorld(pgn::Pool* pool);
	virtual ~PhysicsWorld();
	virtual void init();
	virtual void dispose();
	virtual void _free();
	virtual void addGround(pgn::Physics* phy);
	virtual float getGroundHeight(float x, float z);
	virtual bool rayIntersect(pgn::Float3* rayBegin, pgn::Float3* raydir, pgn::Float3* contactPos);

private:
	pgn::Pool* pool;
	dGeomID addTriMesh(void* vertices, int vertexCount, void* indices, int indexCount);
	dWorldID world;
	dSpaceID space;
	dGeomID ground;
};
