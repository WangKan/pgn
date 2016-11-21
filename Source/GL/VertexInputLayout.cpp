#include <PGN/Common/debug_new.h>
#include <PGN/Platform/UI/GL.h>
#include "Program.h"
#include "RenderingSystem.h"
#include "VertexInputLayout.h"

struct AttribFormat
{
	int size;
	unsigned type;
};

const AttribFormat formats[] =
{
	1, GL_FLOAT,			2, GL_FLOAT,			3, GL_FLOAT,			4, GL_FLOAT,
	1, GL_HALF_FLOAT,		2, GL_HALF_FLOAT,		3, GL_HALF_FLOAT,		4, GL_HALF_FLOAT,
	1, GL_BYTE,				2, GL_BYTE,				3, GL_BYTE,				4, GL_BYTE,
	1, GL_UNSIGNED_BYTE,	2, GL_UNSIGNED_BYTE,	3, GL_UNSIGNED_BYTE,	4, GL_UNSIGNED_BYTE,
	1, GL_SHORT,			2, GL_SHORT,			3, GL_SHORT,			4, GL_SHORT,
	1, GL_UNSIGNED_SHORT,	2, GL_UNSIGNED_SHORT,	3, GL_UNSIGNED_SHORT,	4, GL_UNSIGNED_SHORT
};

VertexInputLayout::VertexInputLayout(Program* program, int numAttribs, const pgn::VertexAttribDesc attribDescs[], const int strides[])
{
	for(int i = 0; i < maxNumAttribs; i++)
	{
		attribs[i].enabled = false;
	}

	for(int i = 0; i < maxNumAttribs; i++)
	{
		inputSlotToLocationMap[i] = -1;
	}

	auto& attribLocationMap = program->attribLocationMap;

	for(int i = 0; i < numAttribs; i++)
	{
		const pgn::VertexAttribDesc* desc = &attribDescs[i];
		auto it = attribLocationMap.find(desc->name);

		if(it != attribLocationMap.end())
		{
			int location = it->second;
			Attribute* attrib = &attribs[location];

			attrib->enabled = true;
			attrib->size = formats[desc->format].size;
			attrib->type = formats[desc->format].type;
			attrib->divisor = desc->instanceDataStepRate;

			inputSlotToLocationMap[i] = location;
		}
	}

	for (int i = 0; i < numAttribs; i++)
	{
		this->strides[i] = strides[i];
	}
}

void VertexInputLayout::dispose()
{
}

pgn::VertexInputLayout* RenderingSystem::createVertexInputLayout(pgn::Program* program, int numAttribs, const pgn::VertexAttribDesc attribDescs[], const int strides[])
{
	return debug_new VertexInputLayout((Program*)program, numAttribs, attribDescs, strides);
}

void VertexInputLayout::_free()
{
	delete this;
}

void VertexInputLayout::destroy()
{
	delete this;
}

void RenderingSystem::setVertexInputLayout(pgn::VertexInputLayout* layout)
{
	vertexInputLayout = (VertexInputLayout*)layout;
	for(int i = 0; i < VertexInputLayout::maxNumAttribs; i++)
	{
		Attribute attrib = ((VertexInputLayout*)layout)->attribs[i];
		if(attrib.enabled)
		{
			glEnableVertexAttribArray(i);
			glVertexAttribDivisor(i, attrib.divisor);
		}
		else
		{
			glDisableVertexAttribArray(i);
		}
	}
}
