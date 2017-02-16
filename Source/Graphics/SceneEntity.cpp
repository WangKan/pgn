#include "SceneEntity.h"

SceneEntity::SceneEntity(Model* model, bool useInstancedDrawing)
{
	this->model = model;
	this->useInstancedDrawing = useInstancedDrawing;
}

SceneEntity::SceneEntity(SkeletalModel* model, bool useInstancedDrawing)
{
	this->skeletalModel = model;
	this->useInstancedDrawing = useInstancedDrawing;
}

SceneEntity::SceneEntity(NavModel* model, bool useInstancedDrawing)
{
	this->navModel = model;
	this->useInstancedDrawing = useInstancedDrawing;
}

void SceneEntity::setScale(float xz, float y)
{
	movable.scaleXZ = xz;
	movable.scaleY = y;
}

void SceneEntity::setWorldMat(pgn::Float4x3* mat)
{
	movable.worldMat = *mat;
}

void SceneEntity::getWorldPos(pgn::Float3& pos)
{
	pos.x = movable.worldMat[0][3];
	pos.y = movable.worldMat[1][3];
	pos.z = movable.worldMat[2][3];
}
