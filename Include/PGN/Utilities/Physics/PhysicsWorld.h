#pragma once
#include "../../Common/Interface.h"

namespace pgn {
struct Float3;
class Physics;
class PhysicsWorld : public Interface
{
public:
	virtual void init() = 0;
	virtual void addGround(Physics* phy) = 0;
	virtual float getGroundHeight(float x, float z) = 0;
	virtual bool rayIntersect(Float3* rayBegin, Float3* rayDir, Float3* contactPos) = 0;
};
}
