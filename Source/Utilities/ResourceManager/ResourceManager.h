#define PGN_DLL_EXPORT
#include <PGN/Utilities/ResourceManager/ResourceManager.h>
#undef PGN_DLL_EXPORT

#include <map>
#define PGN_STATIC_IMPORT
#include <PGN/Utilities/Pool.h>
#undef PGN_STATIC_IMPORT
#include <PGN/Utilities/PoolAllocator.h>
#include "ResourceHandle.h"

class ResourceManager : public pgn::ResourceManager
{
	pgn::AssetFactory* factory;
	pgn::FileStream* f;
	pgn::AssetLoader* loader;
	std::map<std::string, ResourceHandle, std::less<std::string>, pgn::PoolAllocator<std::pair<std::string, ResourceHandle>, pgn::Pool>> resourceMap;

public:
	ResourceManager(pgn::AssetFactory* factory, pgn::FileStream* f, pgn::AssetLoader* loader);
	virtual void dispose();
	virtual void _free();

	virtual pgn::ResourceHandle* getResource(const char name[]);
	virtual void releaseResource(pgn::ResourceHandle* resHandle);

	virtual pgn::ResourceHandle* addResource(const char name[], void* core);
	virtual void removeResource(pgn::ResourceHandle* resHandle);

	virtual void* peekResource(char name[]);

	void collectGarbage(int maxCount, int maxTry = 2);
};
