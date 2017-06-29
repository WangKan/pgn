#include <PGN/Common/debug_new.h>
#include <PGN/Platform/UI/GL.h>
#include <PGN/RenderingSystem/Buffer.h>
#include <PGN/Utilities/SharedInterfaceImpl.h>
#include "RenderingSystem.h"
#include "VertexInputLayout.h"

class Buffer : public virtual pgn::Buffer, public virtual SharedInterfaceImpl
{
public:
	unsigned bindingType;
	unsigned buf;

	Buffer(pgn::BindingType bindingType, pgn::BufferUsage usage, int size);
	virtual void dispose();
	virtual void _free();
	virtual void update(int offset, int size, void* data);
	virtual void* map(int offset, int size);
    virtual void commit(int offset, int size);
	virtual void unmap();
};

const unsigned types[] = {GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_UNIFORM_BUFFER};
const unsigned usages[] = {GL_STATIC_DRAW, GL_DYNAMIC_DRAW};

Buffer::Buffer(pgn::BindingType bindingType, pgn::BufferUsage usage, int size)
{
	this->bindingType = types[bindingType];
	glGenBuffers(1, &buf);
	glBindBuffer(this->bindingType, buf);
	glBufferData(this->bindingType, size, 0, usages[usage]);
}

void Buffer::dispose()
{
	glDeleteBuffers(1, &buf);
}

pgn::Buffer* RenderingSystem::createBuffer(pgn::BufferDesc* desc)
{
	return debug_new Buffer(desc->bindingType, desc->usage, desc->size);
}

void Buffer::_free()
{
	delete this;
}

void Buffer::update(int offset, int size, void* data)
{
	glBindBuffer(bindingType, buf);
	glBufferSubData(bindingType, offset, size, data);
}

void* Buffer::map(int offset, int size)
{
	glBindBuffer(bindingType, buf);
	return glMapBufferRange(bindingType, offset, size, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
}

void Buffer::commit(int offset, int size)
{
    glFlushMappedBufferRange(bindingType, offset, size);
}

void Buffer::unmap()
{
	glUnmapBuffer(bindingType);
}

void RenderingSystem::setVertexBuffers(int startSlot, int numBuffers, pgn::Buffer* buffers[], int offsets[])
{
	Buffer* curVB = 0;

	for(int i = 0, slot = startSlot; i < numBuffers; i++, slot++)
	{
		int location = vertexInputLayout->inputSlotToLocationMap[slot];
		if(location >= 0)
		{
			Attribute attrib = vertexInputLayout->attribs[location];
			Buffer* vb = dynamic_cast<Buffer*>(buffers[i]);
			if(curVB != vb)
			{
				glBindBuffer(GL_ARRAY_BUFFER, vb->buf);
				curVB = vb;
			}
			if(attrib.type == GL_FLOAT || attrib.type == GL_HALF_FLOAT)
			{
				glVertexAttribPointer(location, attrib.size, attrib.type, false, vertexInputLayout->strides[i], (void*)offsets[i]);
			}
			else
			{
				glVertexAttribIPointer(location, attrib.size, attrib.type, vertexInputLayout->strides[i], (void*)offsets[i]);
			}
		}
	}
}

void RenderingSystem::setIndexBuffer(pgn::Buffer* buf, int offset)
{
	indexBufferOffset = offset;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dynamic_cast<Buffer*>(buf)->buf);
}

void RenderingSystem::setConstantBuffers(int startSlot, int numBuffers, pgn::Buffer* buffers[], int offsets[], int sizes[])
{
	for(int i = 0; i < numBuffers; i++)
	{
		int indexBindingPoint = startSlot + i;
		glBindBufferRange(GL_UNIFORM_BUFFER, indexBindingPoint, dynamic_cast<Buffer*>(buffers[i])->buf, offsets[i], sizes[i]);
	}
}
