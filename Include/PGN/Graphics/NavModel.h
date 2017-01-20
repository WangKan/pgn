#pragma once
#include "../Common/Interface.h"
namespace pgn {

struct Float3;

class NavModel : public virtual Interface
{
public:
	virtual void setMesh(char fileName[]) = 0;
	virtual bool complete() = 0;
	virtual void getAabb(Float3* min, Float3* max) = 0;
};

}
