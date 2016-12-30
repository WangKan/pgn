#include <PGN/Math/Math.h>
#include <PGN/Utilities/Pool.h>
#include <PGN/Utilities/ResourceManager/ResourceHandle.h>
#include "PhyMesh.h"
#include "Physics.h"
#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld(pgn::Pool* pool)
{
	this->pool = pool;
}

PhysicsWorld::~PhysicsWorld()
{
}

void PhysicsWorld::init()
{
	dInitODE2(0);
	world = dWorldCreate();
	space = dSimpleSpaceCreate(0);
}

void PhysicsWorld::dispose()
{
}

void PhysicsWorld::_free()
{
	pool->_free(this);
}

void PhysicsWorld::addGround(pgn::Physics* phy)
{
	Physics* p = (Physics*)phy;
	if (!p->complete())
		return;
	PhyMesh* mesh = (PhyMesh*)(p->physics->core());
	ground = addTriMesh(mesh->vertexBuffer, mesh->numVertices, mesh->indexBuffer, mesh->numIndices);
}

dGeomID PhysicsWorld::addTriMesh(void* vertices, int vertexCount, void* indices, int indexCount)
{
	dTriMeshDataID meshData = dGeomTriMeshDataCreate();
	dGeomTriMeshDataBuildSingle(meshData, vertices, 3 * sizeof(float), vertexCount, indices, indexCount, sizeof(unsigned short));
	return dCreateTriMesh(space, meshData, 0, 0, 0);
}

float PhysicsWorld::getGroundHeight(float x, float z)
{
	dGeomID ray = dCreateRay(space, 1000.f);
	dGeomRaySet(ray, x, 1000.0f, z, 0.0f, -1.0f, 0.0f);
	dContactGeom c;
	int result = dCollide(ray, ground, 1, &c, sizeof(dContactGeom));
	if (result > 0)
		return c.pos[1];

	return 0.f;
}

bool PhysicsWorld::rayIntersect(pgn::Float3* rayBegin, pgn::Float3* rayDir, pgn::Float3* contactPos)
{
	dGeomID ray = dCreateRay(space, 1000.f);
	dGeomRaySet(ray, rayBegin->v[0], rayBegin->v[1], rayBegin->v[2], rayDir->v[0], rayDir->v[1], rayDir->v[2]);
	dContactGeom c;
	int result = dCollide(ray, ground, 1, &c, sizeof(dContactGeom));
	if (result > 0)
	{
		contactPos->v[0] = c.pos[0];
		contactPos->v[1] = c.pos[1];
		contactPos->v[2] = c.pos[2];
		return true;
	}
	return false;
}


