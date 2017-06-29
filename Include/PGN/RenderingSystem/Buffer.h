#pragma once
#include "../Common/SharedInterface.h"
namespace pgn {

enum BindingType
{
	VERTEX_BUFFER,
	INDEX_BUFFER,
	CONSTANT_BUFFER
};

enum BufferUsage
{
	STATIC_DRAW, DYNAMIC_DRAW
};

struct BufferDesc
{
	BindingType		bindingType;
	BufferUsage		usage;
	int				size;
};

class Buffer : public virtual SharedInterface
{
public:
	virtual void update(int offset, int size, void* data) = 0;
	virtual void* map(int offset, int size) = 0;
    virtual void commit(int offset, int size) = 0;
	virtual void unmap() = 0;
};

}
