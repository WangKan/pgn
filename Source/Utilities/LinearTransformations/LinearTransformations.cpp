#define PGN_DLL_EXPORT
#include <PGN/Utilities/LinearTransformations.h>
#undef PGN_DLL_EXPORT

#include <PGN/Math/Utilities.h>
#include "sinCosTable.h"

const pgn::Float4 v0001 = { 0, 0, 0, 1 };

void buildRotationMat(pgn::Float3 axis, float angle, pgn::Float4x3& result)
{
	float s, c;
	pgn::sincos(angle, &s, &c);

	float _1_c = 1.0f - c;

	float _1_c_x = _1_c * axis[0];
	float _1_c_y = _1_c * axis[1];
	float _1_c_z = _1_c * axis[2];

	float _1_c_xx = _1_c_x * axis[0];
	float _1_c_yy = _1_c_y * axis[1];
	float _1_c_zz = _1_c_z * axis[2];
	float _1_c_xy = _1_c_x * axis[1];
	float _1_c_yz = _1_c_y * axis[2];
	float _1_c_zx = _1_c_z * axis[0];

	float xs = axis[0] * s;
	float ys = axis[1] * s;
	float zs = axis[2] * s;

	result[0][0] = _1_c_xx + c;		result[1][0] = _1_c_xy + zs;	result[2][0] = _1_c_zx - ys;
	result[0][1] = _1_c_xy - zs;	result[1][1] = _1_c_yy + c;		result[2][1] = _1_c_yz + xs;
	result[0][2] = _1_c_zx + ys;	result[1][2] = _1_c_yz - xs;	result[2][2] = _1_c_zz + c;
	result[0][3] = 0;				result[1][3] = 0;				result[2][3] = 0;
}

void buildRotationXMat(float angle, pgn::Float4x3& result)
{
	float s, c;
	pgn::sincos(angle, &s, &c);

	result[0][0] = 1;	result[1][0] = 0;	result[2][0] = 0;
	result[0][1] = 0;	result[1][1] = c;	result[2][1] = s;
	result[0][2] = 0;	result[1][2] = -s;	result[2][2] = c;
	result[0][3] = 0;	result[1][3] = 0;	result[2][3] = 0;
}

void applyPivot(pgn::Float4x3& m, pgn::Float3& pivot)
{
	m[0][3] = pivot[0];
	m[1][3] = pivot[1];
	m[2][3] = pivot[2];

	pgn::Float3 invTRow4 = -pivot;
	pgn::Float3 resultRow4;
	transformVertex(&invTRow4, &m, &resultRow4);

	m[0][3] = resultRow4[0];
	m[1][3] = resultRow4[1];
	m[2][3] = resultRow4[2];
}

void pgn::buildWorldMat(float x, float y, float z, int yaw, Float4x3* result)
{
	Float4x3& m = *result;

	Float2 sinCos = sinCosTable[yaw];

	m[0][0] = sinCos[1];	m[1][0] = 0;		m[2][0] = -sinCos[0];
	m[0][1] = 0;			m[1][1] = 1;		m[2][1] = 0;
	m[0][2] = sinCos[0];	m[1][2] = 0;		m[2][2] = sinCos[1];
	m[0][3] = x;			m[1][3] = y;		m[2][3] = z;
}

void pgn::buildViewMat(Float3* _eye, Float3* _lookAt, Float3* _up, Float4x3* _result)
{
	Float3& eye = *_eye;
	Float3& lookAt = *_lookAt;
	Float3& up = *_up;
	Float4x3& m = *_result;

	Float3 axisZ = normalize(lookAt - eye);
	Float3 axisX = normalize(cross(up, axisZ));
	Float3 axisY = normalize(cross(axisZ, axisX));

	m[0].float3 = axisX;
	m[1].float3 = axisY;
	m[2].float3 = axisZ;

	Float3 eyeInParallelSpace;
	pgn::transformVector(&eye, &m, &eyeInParallelSpace);

	m[0][3] = -eyeInParallelSpace[0];
	m[1][3] = -eyeInParallelSpace[1];
	m[2][3] = -eyeInParallelSpace[2];
}

void pgn::buildViewMat(Float4x3* baseViewMat, Float3* pivotInWorldSpace, float yaw, float pitch, Float4x3* result)
{
	Float3 pivotInViewSpace;
	transformVertex(pivotInWorldSpace, baseViewMat, &pivotInViewSpace);

	Float3 upVecInBaseViewSpace;
	upVecInBaseViewSpace[0] = (*baseViewMat)[0][1];
	upVecInBaseViewSpace[1] = (*baseViewMat)[1][1];
	upVecInBaseViewSpace[2] = (*baseViewMat)[2][1];

	Float4x3 R1;
	buildRotationMat(upVecInBaseViewSpace, yaw, R1);
	applyPivot(R1, pivotInViewSpace);

	Float4x3 R2;
	buildRotationXMat(pitch, R2);
	applyPivot(R2, pivotInViewSpace);

	Float4x4 B;
	B.float4x3 = *baseViewMat;
	B[3] = v0001;

	Float4x4 BR1;
	mul(&B, &R1, &BR1.float4x3);
	BR1[3] = v0001;

	mul(&BR1, &R2, result);
}

void pgn::calculateInverseViewMat(Float4x3* viewMat, Float4x3* result)
{
	pgn::Float3 invTRow4;
	invTRow4[0] = -(*viewMat)[0][3];
	invTRow4[1] = -(*viewMat)[1][3];
	invTRow4[2] = -(*viewMat)[2][3];

	pgn::inverse(viewMat, result);

	pgn::Float3 row4;
	pgn::transformVector(&invTRow4, result, &row4);

	(*result)[0][3] = row4[0];
	(*result)[1][3] = row4[1];
	(*result)[2][3] = row4[2];
}
