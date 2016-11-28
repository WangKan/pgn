#pragma once
#include "../Common/Interface.h"
namespace pgn {

class NavModel : public virtual Interface
{
public:
	virtual void setMesh(char fileName[]) = 0;
	virtual void* getVertexBuffer() = 0;
	virtual void* getIndexBuffer() = 0;
	virtual void* getAdjacentNodes() = 0;
	virtual int getVertexCount() = 0;
	virtual int getIndexCount() = 0;
};

}
