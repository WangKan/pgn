#pragma once
#include <list>
#include <PGN/Graphics/SceneEntity.h>
#include <PGN/Utilities/Pool.h>
#include <PGN/Utilities/PoolAllocator.h>
#include "Renderer/Movable.h"

class SkeletalModel;

class SceneEntity : public pgn::SceneEntity
{
public:
	typedef pgn::PoolAllocator<SceneEntity, pgn::Pool> StdListAllocator;
	typedef std::list<SceneEntity, StdListAllocator> StdList;

	Movable movable;
	SkeletalModel* skeletalModel;
	bool useInstancedDrawing;
	StdList::iterator it;

	SceneEntity(SkeletalModel* skeletalModel, bool useInstancedDrawing);
	virtual void setScale(float xz, float y);
	virtual void setWorldMat(pgn::Float4x3* mat);
	virtual void getWorldPos(pgn::Float3& pos);
};
