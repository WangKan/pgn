#include <PGN/Assets/PNM.h>
#include <PGN/Math/Math.h>
#include "Geometry.h"

class EditableGeometry : public Geometry
{
public:
	struct Subset
	{
		struct{
			pgn::Float3 min;
			pgn::Float3 max;
		}aabb;
	};

	Subset subsets[pgn::PNMHeader::maxNumSubsets];

	EditableGeometry(SubsetAllocator& subsetAllocator)
		:Geometry(subsetAllocator)
	{
	}
};
