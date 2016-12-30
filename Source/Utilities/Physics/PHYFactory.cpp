#include <new>
#include <PGN/Platform/pageAlloc.h>
#define PGN_STATIC_IMPORT
#include <PGN/Utilities/ManagedHeap.h>
#include <PGN/Utilities/Pool.h>
#undef PGN_STATIC_IMPORT
#include "Phy.h"
#include "PhyFactory.h"

const int heapSize = 1 * 1024 * 1024;

PHYFactory::PHYFactory()
{
	pool = pgn::Pool::create(sizeof(PHY));

	_heap = pgn::pageAlloc(heapSize);
	heap = pgn::ManagedHeap::create(_heap, heapSize, memcpy);
}

PHYFactory::~PHYFactory()
{
	pool->destroy();
	heap->destroy();
	pgn::pageFree(_heap, heapSize);
}

pgn::Asset* PHYFactory::create()
{
	return new(pool->alloc()) PHY(this, heap);
}

void PHYFactory::recycle(pgn::Asset* asset)
{
	((PHY*)asset)->~PHY();
	pool->_free(asset);
}
