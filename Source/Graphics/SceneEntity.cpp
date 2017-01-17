#include "SceneEntity.h"

SceneEntity::SceneEntity(SkeletalModel* skeletalModel, bool useInstancedDrawing)
{
	this->skeletalModel = skeletalModel;
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