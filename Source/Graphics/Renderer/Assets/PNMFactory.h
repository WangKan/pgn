#include <PGN/Utilities/ResourceManager/AssetFactory.h>
#include "../Geometry.h"

namespace pgn {
	class SkeletonTemplateFactory;
	class Pool;
}

class PNMFactory : public pgn::AssetFactory
{
public:
	pgn::Pool* meshPool;
	pgn::Pool* geomPool;
	SubsetAllocator& subsetAllocator;
	pgn::SkeletonTemplateFactory* skeletonTemplateFactory;

	PNMFactory(SubsetAllocator& subsetAllocator);
	~PNMFactory();
	virtual pgn::Asset* create();
	virtual void recycle(pgn::Asset* asset);
};
