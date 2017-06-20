#pragma once
#include "../../Common/DllInterface.h"
#include "../../Common/Interface.h"
namespace pgn {

class AssetFactory;
class AssetLoader;
class FileStream;
class ResourceHandle;

class ResourceManager : public pgn::Interface
{
public:
	PGN_DLL_INTERFACE static ResourceManager* create(AssetFactory* factory, FileStream* f, AssetLoader* loader);

	virtual ResourceHandle* getResource(const char name[]) = 0;
	virtual void releaseResource(ResourceHandle* resHandle) = 0;

	virtual ResourceHandle* addResource(const char name[], void* core) = 0;
	virtual void removeResource(ResourceHandle* resHandle) = 0;

	virtual void* peekResource(char name[]) = 0;
};

}
