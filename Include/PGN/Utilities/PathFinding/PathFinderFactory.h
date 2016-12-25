#pragma once
#include "../../Common/DllInterface.h"
#include "../../Common/Interface.h"
namespace pgn {

class FileStream;
class PathFinder;

class PathFinderFactory : public Interface
{
public:
	static PGN_DLL_INTERFACE PathFinderFactory* create(FileStream* f);

	virtual PathFinder* createPathFinder() = 0;
};

}
