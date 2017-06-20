#include <cassert>
#include <PGN/Platform/Thread/sleep.h>
#include <PGN/Utilities/ResourceManager/Asset.h>
#include <PGN/Utilities/ResourceManager/AssetFactory.h>
#include <PGN/Utilities/ResourceManager/AssetLoader.h>
#include "ResourceManager.h"

ResourceManager::ResourceManager(pgn::AssetFactory* factory, pgn::FileStream* f, pgn::AssetLoader* loader)
{
	this->factory = factory;
	this->f = f;
	this->loader = loader;
}

void ResourceManager::dispose()
{
	loader->finish();
	collectGarbage(100000000, 100000000);
	assert(resourceMap.empty());
}

pgn::ResourceManager* pgn::ResourceManager::create(AssetFactory* factory, pgn::FileStream* f, AssetLoader* loader)
{
	return debug_new ::ResourceManager(factory, f, loader);
}

void ResourceManager::_free()
{
	delete this;
}

pgn::ResourceHandle* ResourceManager::getResource(const char name[])
{
	collectGarbage(1);

	std::string _name(name);
	ResourceHandle* resHandle = &resourceMap[_name];

	if (resHandle->refCount == 0)
	{
		pgn::Asset* asset = factory->create();
		resHandle->asset = asset;
		resHandle->it = resourceMap.find(_name);

		while (!loader->load(f, name, asset))
		{
			pgn::sleep(10);
			collectGarbage(1);
		}
	}

	resHandle->refCount++;
	return resHandle;
}

void ResourceManager::releaseResource(pgn::ResourceHandle* _resHandle)
{
	collectGarbage(1);

	ResourceHandle* resHandle = (ResourceHandle*)_resHandle;

	if (--resHandle->refCount == 0)
	{
		while (!loader->unload(resHandle->asset))
		{
			pgn::sleep(10);
			collectGarbage(1);
		}
		resourceMap.erase(resHandle->it);
	}
}

const int internalResInitialRefCount = 1;

pgn::ResourceHandle* ResourceManager::addResource(const char name[], void* core)
{
	ResourceHandle* resHandle = &resourceMap[name];

	resHandle->it = resourceMap.find(name);
	resHandle->_core = core;

	if (resHandle->refCount == 0)
		resHandle->refCount = internalResInitialRefCount;

	return resHandle;
}

void ResourceManager::removeResource(pgn::ResourceHandle* _resHandle)
{
	ResourceHandle* resHandle = (ResourceHandle*)_resHandle;

	assert(resHandle->refCount == internalResInitialRefCount);

	resourceMap.erase(resHandle->it);
}

void* ResourceManager::peekResource(char name[])
{
	return resourceMap[name].core();
}

void ResourceManager::collectGarbage(int maxCount, int maxTry)
{
	for (int i = 0; i < maxCount; i++)
	{
		pgn::Asset* garbage = loader->getGarbage(maxTry);

		if (!garbage)
			break;

		garbage->getFactory()->recycle(garbage);
	}
}
