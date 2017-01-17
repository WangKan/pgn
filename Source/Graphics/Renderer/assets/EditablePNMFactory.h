#include <PGN/Utilities/ResourceManager/AssetFactory.h>
#include "../Geometry.h"

namespace pgn {
	class SkeletonTemplateFactory;
	class Pool;
}

class EditablePNMFactory : public pgn::AssetFactory
{
public:
	pgn::Pool* meshPool;
	pgn::Pool* geomPool;
	SubsetAllocator& subsetAllocator;
	pgn::SkeletonTemplateFactory* skeletonTemplateFactory;

	EditablePNMFactory(SubsetAllocator& subsetAllocator);
	~EditablePNMFactory();
	virtual pgn::Asset* create();
	virtual void recycle(pgn::Asset* asset);
};
