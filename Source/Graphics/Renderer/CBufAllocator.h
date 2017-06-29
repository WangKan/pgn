#include <PGN/RenderingSystem/Buffer.h>
#include <PGN/RenderingSystem/RenderingSystem.h>
#include <vector>
#include "CBufRange.h"

class CBufAllocator
{
public:
	class Buffer
	{
		static const int sizeBuf = 16 * 1024;
		pgn::Buffer* buf;
		int offset;
		int offsetAlignment_1;

	public:

		Buffer(pgn::RenderingSystem* rs)
		{
			static pgn::BufferDesc bufDesc =
			{
				pgn::CONSTANT_BUFFER,
				pgn::DYNAMIC_DRAW,
				sizeBuf
			};

			buf = rs->createBuffer(&bufDesc);

			offset = 0;
			offsetAlignment_1 = rs->getConstantBufferOffsetAlignment() - 1;
		}

		Buffer(const Buffer& src)
		{
			*this = src;
			buf->retain();
		}

		~Buffer()
		{
			buf->release();
		}

		void* map()
		{
			return buf->map(0, sizeBuf);
		}

		void unmap()
        {
            buf->commit(0, offset);
			buf->unmap();
		}

		bool alloc(int size, CBufRange* bufRange)
		{
			size = (size + offsetAlignment_1) & ~offsetAlignment_1;

			int _offset = offset;
			offset += size;

			if (offset > sizeBuf)
				return false;

			bufRange->buf = buf;
			bufRange->offset = _offset;
			bufRange->size = size;
			return true;
		}

		void clear()
		{
			offset = 0;
		}
	};

	pgn::RenderingSystem* rs;
	std::vector<Buffer> bufs;
	int curBuf;
	void* baseAddr;

	CBufAllocator(pgn::RenderingSystem* rs)
	{
		this->rs = rs;
		bufs.emplace_back(rs);
        baseAddr = 0;
	}

	~CBufAllocator()
	{
		bufs.clear();
	}

	void init()
	{
		for (auto& buf : bufs)
			buf.clear();

		curBuf = 0;
	}

	void* alloc(int size, CBufRange* bufRange)
	{
        if (!baseAddr)
            baseAddr = bufs[curBuf].map();
        
		while (!bufs[curBuf].alloc(size, bufRange))
		{
			bufs[curBuf].unmap();
            
			curBuf++;

			if (curBuf == bufs.size())
				bufs.emplace_back(rs);

			baseAddr = bufs[curBuf].map();
		}

		return ((char*)baseAddr) + bufRange->offset;
	}

	void commit()
	{
        bufs[curBuf].unmap();
        baseAddr = 0;
    }
};
