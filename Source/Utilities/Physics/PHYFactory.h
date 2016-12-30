#define PGN_STATIC_IMPORT
#include <PGN/Utilities/ResourceManager/AssetFactory.h>
#undef PGN_STATIC_IMPORT

namespace pgn {
	class ManagedHeap;
	class Pool;
}

class PHYFactory : public pgn::AssetFactory
{
public:
	pgn::Pool* pool;
	void* _heap;
	pgn::ManagedHeap* heap;

	PHYFactory();
	~PHYFactory();
	virtual pgn::Asset* create();
	virtual void recycle(pgn::Asset* asset);
};
