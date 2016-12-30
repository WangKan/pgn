#pragma once
#include <queue>
#include "../../Common/Interface.h"

namespace pgn {
struct Float3;
class PathFinder : public Interface
{
public:
	virtual void setNavigationMesh(char fileName[]) = 0;
	virtual bool ready() = 0;
	virtual void build() = 0;
	virtual bool search(pgn::Float3& start, pgn::Float3& end, std::deque<pgn::Float3>& path) = 0;
};

}
