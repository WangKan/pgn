#pragma once
#include <math.h>
#include "Math.h"

inline float clamp(float n, float min, float max)
{
	if (n < min) return min;
	if (n > max) return max;
	return n;
}


inline bool operator==(pgn::Float2& a, pgn::Float2& b)
{
	return a.x == b.x && a.y == b.y;
}

inline bool operator!=(pgn::Float2& a, pgn::Float2& b)
{
	return a.x != b.x || a.y != b.y;
}

inline pgn::Float2 operator-(pgn::Float2& v)
{
	return pgn::Float2(-v.x, -v.y);
}

inline pgn::Float2 operator+(const pgn::Float2& a, const pgn::Float2& b)
{
	return pgn::Float2(a.x + b.x, a.y + b.y);
}

inline pgn::Float2 operator-(pgn::Float2& a, pgn::Float2& b)
{
	return pgn::Float2(a.x - b.x, a.y - b.y);
}

inline pgn::Float2 operator*(pgn::Float2& v, float t)
{
	return pgn::Float2(v.x * t, v.y * t);
}

inline float dot(pgn::Float2& a, pgn::Float2& b)
{
	return a.x * b.x + a.y * b.y;
}

inline float cross(const pgn::Float2& a, const pgn::Float2& b)
{
	return a.x * b.y - a.y * b.x;
}

inline float length(pgn::Float2& v)
{
	return sqrt(dot(v, v));
}

inline pgn::Float2 normalize(pgn::Float2& v)
{
	float invLen = 1.0f / length(v);
	return v * invLen;
}


inline bool operator==(pgn::Float3& a, pgn::Float3& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline bool operator!=(pgn::Float3& a, pgn::Float3& b)
{
	return a.x != b.x || a.y != b.y || a.z != b.z;
}

inline pgn::Float3 operator-(pgn::Float3& v)
{
	pgn::Float3 r = { -v.x, -v.y, -v.z };
	return r;
}

inline pgn::Float3 operator+(pgn::Float3& a, pgn::Float3& b)
{
	pgn::Float3 r = { a.x + b.x, a.y + b.y, a.z + b.z };
	return r;
}

inline pgn::Float3 operator-(pgn::Float3& a, pgn::Float3& b)
{
	pgn::Float3 r = { a.x - b.x, a.y - b.y, a.z - b.z };
	return r;
}

inline pgn::Float3 operator*(const pgn::Float3& v, float t)
{
	pgn::Float3 r = { v.x * t, v.y * t, v.z * t };
	return r;
}

inline float dot(const pgn::Float3& a, const pgn::Float3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline pgn::Float3 cross(pgn::Float3& a, pgn::Float3& b)
{
	pgn::Float3 r;
	r.x = a.y * b.z - a.z * b.y;
	r.y = a.z * b.x - a.x * b.z;
	r.z = a.x * b.y - a.y * b.x;

	return r;
}

inline float length(const pgn::Float3& v)
{
	return sqrt(dot(v, v));
}

inline pgn::Float3 normalize(const pgn::Float3& v)
{
	float invLen = 1.0f / length(v);
	return v * invLen;
}
