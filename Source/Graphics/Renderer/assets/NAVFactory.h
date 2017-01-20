#include <PGN/Utilities/ResourceManager/AssetFactory.h>
#include "../Geometry.h"

namespace pgn {
	class Pool;
}

class NAVFactory : public pgn::AssetFactory
{
public:
	pgn::Pool* meshPool;
	pgn::Pool* geomPool;
	SubsetAllocator& subsetAllocator;

	NAVFactory(SubsetAllocator& subsetAllocator);
	~NAVFactory();
	virtual pgn::Asset* create();
	virtual void recycle(pgn::Asset* asset);
};
