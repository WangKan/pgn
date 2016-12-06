#pragma once
#include "../Common/aligned.h"
#include "../Common/DllInterface.h"
namespace pgn {

struct Float2
{
	_aligned(8, float v[2]);

	Float2()
	{

	}

	Float2(float x, float y)
	{
		v[0] = x;
		v[1] = y;
	}

	float& operator[](int i)
	{
		return v[i];
	}
};

struct Float3
{
	_aligned(16, float v[3]);

	float& operator[](int i)
	{
		return v[i];
	}
};

struct Float4
{
	union
	{
		_aligned(16, float v[4]);
		Float3 float3;
	};

	float& operator[](int i)
	{
		return v[i];
	}
};

struct Float4x3
{
	Float4 col[3];

	Float4& operator[](int i)
	{
		return col[i];
	}
};

struct Float4x4
{
	union
	{
		Float4 col[4];
		Float4x3 float4x3;
	};

	Float4& operator[](int i)
	{
		return col[i];
	}
};

struct Float3x2
{
	Float3 col[2];

	Float3& operator[](int i)
	{
		return col[i];
	}
};

PGN_DLL_INTERFACE void mul(Float4x4* a, Float4x4* b, Float4x4* result);
PGN_DLL_INTERFACE void mul(Float4x4* a, Float4x3* b, Float4x3* result);
PGN_DLL_INTERFACE void mul(Float4x3* a, Float4x3* b, Float4x3* result); // 3x3æÿ’Û≥À∑®

PGN_DLL_INTERFACE void lerp(Float3* a, Float3* b, float t, Float3* result);
PGN_DLL_INTERFACE void slerp(Float4* a, Float4* b, float t, Float4* result);

PGN_DLL_INTERFACE void quatToMat(Float4* q, Float4x3* m);

PGN_DLL_INTERFACE void add(Float4* a, Float4* b, Float4* results, int count);
PGN_DLL_INTERFACE void sub(Float4* a, Float4* b, Float4* results, int count);

PGN_DLL_INTERFACE bool pointInCCWTriangle(Float2* p, Float2* a, Float2* b, Float2* c);

PGN_DLL_INTERFACE void closestPtSegmentSegment(Float2* p1, Float2* q1, Float2* p2, Float2* q2, Float2* c1, Float2* c2);

PGN_DLL_INTERFACE void precomputeYTriangle(Float2* Axz, Float2* Bxz, Float2* Cxz, float Ay, float By, float Cy, Float4* intermediateVars1, Float2* intermediateVars2);
PGN_DLL_INTERFACE float computeYTriangle(Float4* intermediateVars1, Float2* intermediateVars2, Float2* xz);

PGN_DLL_INTERFACE bool equals(Float3& a, Float3& b);
PGN_DLL_INTERFACE void normalize3d(Float3& a);
PGN_DLL_INTERFACE float length3d(Float3& a, Float3& b);
PGN_DLL_INTERFACE void cross3d(Float3& result, Float3& a, Float3& b);
PGN_DLL_INTERFACE float dot3d(Float3& a, Float3& b);
PGN_DLL_INTERFACE bool pointInTriangle(Float3& p, Float3& a, Float3& b, Float3& c);
}
