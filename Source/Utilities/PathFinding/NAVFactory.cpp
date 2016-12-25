#include <new>
#include <PGN/Platform/pageAlloc.h>
#define PGN_STATIC_IMPORT
#include <PGN/Utilities/ManagedHeap.h>
#include <PGN/Utilities/Pool.h>
#undef PGN_STATIC_IMPORT
#include "NAV.h"
#include "NAVFactory.h"

const int heapSize = 1 * 1024 * 1024;

NAVFactory::NAVFactory()
{
	pool = pgn::Pool::create(sizeof(NAV));

	_heap = pgn::pageAlloc(heapSize);
	heap = pgn::ManagedHeap::create(_heap, heapSize, memcpy);
}

NAVFactory::~NAVFactory()
{
	pool->destroy();
	heap->destroy();
	pgn::pageFree(_heap, heapSize);
}

pgn::Asset* NAVFactory::create()
{
	return new(pool->alloc()) NAV(this, heap);
}

void NAVFactory::recycle(pgn::Asset* asset)
{
	((NAV*)asset)->~NAV();
	pool->_free(asset);
}
