#include "Entity.h"
#include "Graphics.h"
#include "Model.h"

Entity::Entity(pgn::Pool* pool)
{
	this->pool = pool;
	model = 0;
	skel = 0;
	boneMats = 0;
}

void Entity::dispose()
{
}

pgn::Entity* Graphics::createEntity()
{
	return new(entityPool->alloc()) Entity(entityPool);
}

void Entity::_free()
{
	pool->_free(this);
}

void Entity::setModel(pgn::Model* model)
{
	this->model = dynamic_cast<Model*>(model);
}

void Entity::setSkeleton(pgn::Skeleton* skel)
{
	this->skel = skel;
}

void Entity::setPose(pgn::Float4x3* boneMats)
{
	this->boneMats = boneMats;
}
