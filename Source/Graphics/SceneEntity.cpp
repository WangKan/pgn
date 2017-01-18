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
	pos[0] = movable.worldMat[0][3];
	pos[1] = movable.worldMat[1][3];
	pos[2] = movable.worldMat[2][3];
}