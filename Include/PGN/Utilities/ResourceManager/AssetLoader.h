#pragma once
namespace pgn {

class Asset;
class FileStream;

class AssetLoader
{
public:
	virtual bool load(FileStream* f, const char name[], Asset* asset) = 0;
	virtual bool unload(Asset* asset) = 0;
	virtual Asset* getGarbage(int maxTry) = 0;
	virtual void finish() = 0;
};

}
