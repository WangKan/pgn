#pragma once
#include <ode/ode.h>

class PhysicsWorld
{
public:
	PhysicsWorld();
	virtual ~PhysicsWorld();
	void init();
	void destroy();
	void addGround(void* vertices, int vertexCount, void* indices, int indexCount);
	dGeomID addTriMesh(void* vertices, int vertexCount, void* indices, int indexCount);
	float getGroundHeight(float x, float z);
	bool rayIntersectGround(pgn::Float3& rayBegin, pgn::Float3& raydir, pgn::Float3& contactPos);

private:
	dWorldID world;
	dSpaceID space;
	dGeomID ground;
};
