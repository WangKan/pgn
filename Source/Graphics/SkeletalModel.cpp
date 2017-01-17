#include "Graphics.h"
#include "Model.h"
#include "SkeletalModel.h"

SkeletalModel::SkeletalModel(pgn::Pool* pool)
{
	this->pool = pool;
	model = 0;
	skel = 0;
	boneMats = 0;
}

void SkeletalModel::dispose()
{
}

pgn::SkeletalModel* Graphics::createSkeletalModel()
{
	return new(skeletalModelPool->alloc()) SkeletalModel(skeletalModelPool);
}

void SkeletalModel::_free()
{
	pool->_free(this);
}

void SkeletalModel::setModel(pgn::Model* model)
{
	this->model = dynamic_cast<Model*>(model);
}

void SkeletalModel::setSkeleton(pgn::Skeleton* skel)
{
	this->skel = skel;
}

void SkeletalModel::setPose(pgn::Float4x3* boneMats)
{
	this->boneMats = boneMats;
}
