#include <PGN/Utilities/ResourceManager/Asset.h>

class Asset : public pgn::Asset
{
	pgn::AssetFactory* factory;

public:
	Asset(pgn::AssetFactory* factory)
	{
		this->factory = factory;
	}

	virtual pgn::AssetFactory* getFactory()
	{
		return factory;
	}
};
