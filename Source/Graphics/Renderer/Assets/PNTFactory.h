#include <PGN/Utilities/ResourceManager/AssetFactory.h>

namespace pgn {
	class Pool;
}

class PNTFactory : public pgn::AssetFactory
{
public:
	pgn::Pool* pool;

	PNTFactory();
	~PNTFactory();
	virtual pgn::Asset* create();
	virtual void recycle(pgn::Asset* asset);
};
