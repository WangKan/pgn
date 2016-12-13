#pragma once
#include "../Common/DllInterface.h"
#include "Math.h"
namespace pgn{

class PGN_DLL_INTERFACE Plane
{
public:
	Plane();

	bool getIntersectionWithLine(Float3& linePoint, Float3& lineVect, Float3& outIntersection);
	bool getIntersectionWithPlane(Plane& other, Float3& outLinePoint, Float3& outLineVect);
	bool getIntersectionWithPlanes(Plane& o1, Plane& o2, Float3& outPoint);

	Float3 normal;
	float distance;
};

}