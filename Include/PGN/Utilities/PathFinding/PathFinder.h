#pragma once
#include "../../Common/Interface.h"
namespace pgn {

class PathFinder : public Interface
{
public:
	virtual void setNavigationMesh(char fileName[]) = 0;
	virtual bool ready() = 0;
};

}
