#pragma once
#include "../../Common/DllInterface.h"
#include "../../Common/Interface.h"
namespace pgn {

class Physics;
class FileStream;

class PhysicsFactory : public Interface
{
public:
	static PGN_DLL_INTERFACE PhysicsFactory* create(FileStream* f);

	virtual Physics* createPhysics() = 0;
};

}
