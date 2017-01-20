#include "Asset.h"

class NavGeometry;

class NAV : public Asset
{
public:
	NavGeometry* geom;
	bool ready;

	NAV(pgn::AssetFactory* factory, NavGeometry* geom);
	virtual bool cook(void* rawData);
	virtual bool submit(void* rawData, void* customArg);
	virtual void unload(void* customArg);
	virtual void* core();
};
