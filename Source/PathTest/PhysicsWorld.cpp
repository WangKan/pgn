#include <PGN/FileStream/StdFileStream.h>
#include <PGN/Graphics/Camera.h>
#include <PGN/Graphics/Entity.h>
#include <PGN/Graphics/Graphics.h>
#include <PGN/Graphics/NavModel.h>
#include <PGN/Graphics/Model.h>
#include <PGN/Graphics/PointLight.h>
#include <PGN/Graphics/Scene.h>
#include <PGN/Graphics/SceneEntity.h>
#include <PGN/Graphics/ScenePointLight.h>
#include <PGN/Math/Math.h>
#include <PGN/Platform/DebugHeap.h>
#include <PGN/Platform/UI/Window.h>
#include <PGN/Utilities/SkeletalAnimation/Animation.h>
#include <PGN/Utilities/SkeletalAnimation/AnimationFactory.h>
#include <PGN/Utilities/SkeletalAnimation/Skeleton.h>
#include <PGN/Utilities/SkeletalAnimation/SkeletonFactory.h>
#include <PGN/Utilities/SkeletalAnimation/SkeletonTemplate.h>
#include <PGN/Utilities/Clock.h>
#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld()
{
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

void PhysicsWorld::destroy()
{
}

void PhysicsWorld::addGround(void* vertices, int vertexCount, void* indices, int indexCount)
{
	ground = addTriMesh(vertices, vertexCount, indices, indexCount);
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

bool PhysicsWorld::rayIntersectGround(pgn::Float3& rayBegin, pgn::Float3& rayDir, pgn::Float3& contactPos)
{
	dGeomID ray = dCreateRay(space, 1000.f);
	dGeomRaySet(ray, rayBegin[0], rayBegin[1], rayBegin[2], rayDir[0], rayDir[1], rayDir[2]);
	dContactGeom c;
	int result = dCollide(ray, ground, 1, &c, sizeof(dContactGeom));
	if (result > 0)
	{
		contactPos[0] = c.pos[0];
		contactPos[1] = c.pos[1];
		contactPos[2] = c.pos[2];
		return true;
	}
	return false;
}


