#define PGN_DLL_EXPORT
#include <PGN/Math/Plane.h>
#undef PGN_DLL_EXPORT
#include <PGN/Math/Utilities.h>
#include <cmath>

namespace pgn
{
	Plane::Plane()
	{
		normal = { 0.0f, 1.0f, 0.0f };
		distance = 0.0f;

	}

	bool Plane::getIntersectionWithLine(Float3& linePoint, Float3& lineVect, Float3& outIntersection)
	{
		float t2 = dot(normal, lineVect);

		if (t2 == 0)
			return false;

		float t = -(dot(normal, linePoint) + distance) / t2;
		outIntersection.x = linePoint.x + t*lineVect.x;
		outIntersection.y = linePoint.y + t*lineVect.y;
		outIntersection.z = linePoint.z + t*lineVect.z;

		return true;
	}

	bool Plane::getIntersectionWithPlane(Plane& other, Float3& outLinePoint, Float3& outLineVect)
	{
		float fn00 = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
		float fn01 = dot(normal, other.normal);
		float fn11 = sqrt(other.normal.x * other.normal.x + other.normal.y * other.normal.y + other.normal.z * other.normal.z);
		float det = fn00*fn11 - fn01*fn01;

		if (fabs(det) < 1e-6)
			return false;

		float invdet = 1.0f / det;
		float fc0 = (fn11*-distance + fn01*other.distance) * invdet;
		float fc1 = (fn00*-other.distance + fn01*distance) * invdet;

		outLineVect = cross(normal, other.normal);
		outLinePoint.x = fc0*normal.x + fc1*other.normal.x;
		outLinePoint.y = fc0*normal.y + fc1*other.normal.y;
		outLinePoint.z = fc0*normal.z + fc1*other.normal.z;
		
		return true;
	}

	bool Plane::getIntersectionWithPlanes(Plane& o1, Plane& o2, Float3& outPoint)
	{
		Float3 linePoint, lineVect;
		if (getIntersectionWithPlane(o1, linePoint, lineVect))
			return o2.getIntersectionWithLine(linePoint, lineVect, outPoint);

		return false;
	}
}