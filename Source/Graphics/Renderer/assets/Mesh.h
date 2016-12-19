#include "Asset.h"

namespace pgn {

class SkeletonTemplateFactory;

}

class Geometry;

class Mesh : public Asset
{
public:
	Geometry* geom;
	pgn::SkeletonTemplateFactory* skeletonFactory;
	bool ready;

	Mesh(pgn::AssetFactory* factory, Geometry* geom, pgn::SkeletonTemplateFactory* skeletonFactory);
	virtual bool cook(void* rawData);
	virtual bool submit(void* rawData, void* customArg);
	virtual void unload(void* customArg);
	virtual void* core();
};
