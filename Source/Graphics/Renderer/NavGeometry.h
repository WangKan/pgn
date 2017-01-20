#include <PGN/Math/Math.h>
#include "Geometry.h"

class NavGeometry : public Geometry
{
public:
	struct{
		pgn::Float3 min;
		pgn::Float3 max;
	}aabb;

	NavGeometry(SubsetAllocator& subsetAllocator)
		:Geometry(subsetAllocator)
	{
	}
};
