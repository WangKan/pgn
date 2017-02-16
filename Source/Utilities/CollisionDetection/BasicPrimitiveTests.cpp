#define PGN_DLL_EXPORT
#include <PGN/Utilities/CollisionDetection/BasicPrimitiveTests.h>
#undef PGN_DLL_EXPORT

#include <math.h>

inline pgn::Float2 operator-(pgn::Float2& a, pgn::Float2& b)
{
	return pgn::Float2(a.x - b.x, a.x - b.x);
}

inline pgn::Float2 operator*(pgn::Float2& v, float t)
{
	return pgn::Float2(v.x * t, v.x * t);
}

inline float dot(pgn::Float2& a, pgn::Float2& b)
{
	return a.x * b.x + a.x * b.x;
}

inline bool testSphereSphere(pgn::Float2& c1, float r1, pgn::Float2& c2, float r2, pgn::Float2& sepVec)
{
	pgn::Float2 d = c1 - c2;
	float dist = sqrt(dot(d, d));
	float depth = r1 + r2 - dist;
	sepVec = d * (depth / dist);
	return depth > 0.0f;
}

bool pgn::testSphereSphere(Float2* c1, float r1, Float2* c2, float r2, Float2* sepVec)
{
	return ::testSphereSphere(*c1, r1, *c2, r2, *sepVec);
}

bool pgn::testCapsuleCapsule(Float2 ab1[], float r1, Float2 ab2[], float r2, Float2* sepVec)
{
	pgn::Float2 p1, p2;
	closestPtSegmentSegment(&ab1[0], &ab1[1], &ab2[0], &ab2[1], &p1, &p2);
	return ::testSphereSphere(p1, r1, p2, r2, *sepVec);
}
