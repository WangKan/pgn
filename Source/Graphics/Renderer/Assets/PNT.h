#include "Asset.h"

namespace pgn {
	class Texture;
}

class PNT : public Asset
{
	pgn::Texture* tex;
public:
	PNT(pgn::AssetFactory* factory);
	virtual bool cook(void* rawData);
	virtual bool submit(void* rawData, void* customArg);
	virtual void unload(void* customArg);
	virtual void* core();
};
