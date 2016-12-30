#pragma once
#include "../../Common/DllInterface.h"
#include "../../Common/Interface.h"
namespace pgn {

class PhysicsWorld;
class PhysicsWorldFactory : public Interface
{
public:
	static PGN_DLL_INTERFACE PhysicsWorldFactory* create();

	virtual PhysicsWorld* createPhysicsWorld() = 0;
};

}
