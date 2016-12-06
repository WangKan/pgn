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
	return a[0] == b[0] && a[1] == b[1];
}

inline bool operator!=(pgn::Float2& a, pgn::Float2& b)
{
	return a[0] != b[0] || a[1] != b[1];
}

inline pgn::Float2 operator-(pgn::Float2& v)
{
	return pgn::Float2(-v[0], -v[1]);
}

inline pgn::Float2 operator+(pgn::Float2& a, pgn::Float2& b)
{
	return pgn::Float2(a[0] + b[0], a[1] + b[1]);
}

inline pgn::Float2 operator-(pgn::Float2& a, pgn::Float2& b)
{
	return pgn::Float2(a[0] - b[0], a[1] - b[1]);
}

inline pgn::Float2 operator*(pgn::Float2& v, float t)
{
	return pgn::Float2(v[0] * t, v[1] * t);
}

inline float dot(pgn::Float2& a, pgn::Float2& b)
{
	return a[0] * b[0] + a[1] * b[1];
}

inline float cross(pgn::Float2& a, pgn::Float2& b)
{
	return a[0] * b[1] - a[1] * b[0];
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
	return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}

inline bool operator!=(pgn::Float3& a, pgn::Float3& b)
{
	return a[0] != b[0] || a[1] != b[1] || a[2] != b[2];
}

inline pgn::Float3 operator-(pgn::Float3& v)
{
	pgn::Float3 r = { -v[0], -v[1], -v[2] };
	return r;
}

inline pgn::Float3 operator+(pgn::Float3& a, pgn::Float3& b)
{
	pgn::Float3 r = { a[0] + b[0], a[1] + b[1], a[2] + b[2] };
	return r;
}

inline pgn::Float3 operator-(pgn::Float3& a, pgn::Float3& b)
{
	pgn::Float3 r = { a[0] - b[0], a[1] - b[1], a[2] - b[2] };
	return r;
}

inline pgn::Float3 operator*(pgn::Float3& v, float t)
{
	pgn::Float3 r = { v[0] * t, v[1] * t, v[2] * t };
	return r;
}

inline float dot(pgn::Float3& a, pgn::Float3& b)
{
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

inline pgn::Float3 cross(pgn::Float3& a, pgn::Float3& b)
{
	pgn::Float3 r;
	r[0] = a[1] * b[2] - a[2] * b[1];
	r[1] = a[2] * b[0] - a[0] * b[2];
	r[2] = a[0] * b[1] - a[1] * b[0];

	return r;
}

inline float length(pgn::Float3& v)
{
	return sqrt(dot(v, v));
}

inline pgn::Float3 normalize(pgn::Float3& v)
{
	float invLen = 1.0f / length(v);
	return v * invLen;
}
