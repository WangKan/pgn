#define PGN_DLL_EXPORT
#include <PGN/Utilities/Pool.h>
#undef PGN_DLL_EXPORT

#include <cassert>
#include <list>
#include <PGN/Common/MemAlign.h>
#include <PGN/Common/debug_new.h>
#include <PGN/Platform/pageAlloc.h>
#include <vector>

namespace {

class Chunk
{
public:
	size_t capacity;
	std::vector<void*> freeList;
	_aligned(16, char chunk[1]);

	Chunk(size_t blockSize);
	~Chunk();
	void* alloc();
	void _free(void* p);
	bool full();
	bool empty();
};

}

class BaseChunk
{
public:
	static size_t pageSize;
	static size_t size;

	BaseChunk()
	{
		pageSize = pgn::getPageSize();
		size = pageSize - offsetof(Chunk, chunk);
	}
};

size_t BaseChunk::pageSize;
size_t BaseChunk::size;
static BaseChunk baseChunk;

Chunk::Chunk(size_t blockSize)
{
	capacity = BaseChunk::size / blockSize;
	freeList.resize(capacity);
	for (unsigned i = 0; i < capacity; i++)
		freeList[i] = chunk + blockSize * i;
}

Chunk::~Chunk()
{
	assert(empty());
}

void* Chunk::alloc()
{
	void* p = freeList.back();
	freeList.pop_back();
	return p;
}

void Chunk::_free(void* p)
{
	freeList.push_back(p);
}

bool Chunk::full()
{
	return freeList.empty();
}

bool Chunk::empty()
{
	return freeList.size() == capacity;
}

class Pool : public pgn::Pool
{
	size_t blockSize;
	std::list<Chunk*> availableChunks;
public:
	Pool(size_t blockSize);
	virtual void dispose();
	virtual void _free();
	virtual void* alloc();
	virtual void _free(void* p);
	virtual void shrink();
};

Pool::Pool(size_t blockSize)
{
	this->blockSize = blockSize;
	availableChunks.push_back(new(_pageAlloc(BaseChunk::pageSize)) Chunk(blockSize));
}

void Pool::dispose()
{
	for (auto chunk : availableChunks)
	{
		chunk->~Chunk();
		pgn::pageFree(chunk, BaseChunk::pageSize);
	}
}

pgn::Pool* pgn::Pool::create(size_t blockSize)
{
	return debug_new ::Pool(blockSize);
}

void Pool::_free()
{
	delete this;
}

void* Pool::alloc()
{
	if(availableChunks.empty())
		availableChunks.push_back(new(_pageAlloc(BaseChunk::pageSize)) Chunk(blockSize));

	Chunk* chunk = availableChunks.front();
	void* p = chunk->alloc();

	if(chunk->full())
		availableChunks.pop_front();

	return p;
}

void Pool::_free(void* p)
{
	Chunk* chunk = (Chunk*)(((size_t)p) & (~(BaseChunk::pageSize - 1)));

	if(chunk->full())
		availableChunks.push_front(chunk);

	chunk->_free(p);
}

void Pool::shrink()
{
	auto it = availableChunks.begin();
	while(it != availableChunks.end())
	{
		Chunk* chunk = *it;
		if(chunk->empty() && availableChunks.size() > 1)
		{
			chunk->~Chunk();
			pgn::pageFree(chunk, BaseChunk::pageSize);
			it = availableChunks.erase(it);
		}
		else
		{
			it++;
		}
	}
}
