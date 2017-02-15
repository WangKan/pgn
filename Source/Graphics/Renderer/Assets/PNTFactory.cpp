#include <new>
#include <PGN/Utilities/Pool.h>
#include "PNT.h"
#include "PNTFactory.h"

PNTFactory::PNTFactory()
{
	pool = pgn::Pool::create(sizeof(PNT));
}

PNTFactory::~PNTFactory()
{
	pool->destroy();
}

pgn::Asset* PNTFactory::create()
{
	return new(pool->alloc()) PNT(this);
}

void PNTFactory::recycle(pgn::Asset* asset)
{
	pool->_free(asset);
}
