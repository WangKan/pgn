#include <math.h>
#include <PGN/Math/Math.h>

#ifdef PGN_USE_D3D
void buildPerspectiveProjMatLH(pgn::Float4x4* _mat, float w, float h, float n, float f)
{
	pgn::Float4x4& mat = *_mat;

	mat[0][0] = 2*n / w;	mat[1][0] = 0;			mat[2][0] = 0;				mat[3][0] = 0;
	mat[0][1] = 0;			mat[1][1] = 2*n / h;	mat[2][1] = 0;				mat[3][1] = 0;
	mat[0][2] = 0;			mat[1][2] = 0;			mat[2][2] = f / (f - n);	mat[3][2] = 1;
	mat[0][3] = 0;			mat[1][3] = 0;			mat[2][3] = -f*n / (f - n);	mat[3][3] = 0;
}
#else
void buildPerspectiveProjMatLH(pgn::Float4x4* _mat, float w, float h, float n, float f)
{
	pgn::Float4x4& mat = *_mat;

	mat[0][0] = 2*n / w;	mat[1][0] = 0;			mat[2][0] = 0;					mat[3][0] = 0;
	mat[0][1] = 0;			mat[1][1] = 2*n / h;	mat[2][1] = 0;					mat[3][1] = 0;
	mat[0][2] = 0;			mat[1][2] = 0;			mat[2][2] = (f + n) / (f - n);	mat[3][2] = 1;
	mat[0][3] = 0;			mat[1][3] = 0;			mat[2][3] = -2*f*n / (f - n);	mat[3][3] = 0;
}
#endif

void buildPerspectiveProjMatFovLH(pgn::Float4x4* mat, float verticalFov, float aspectRatio, float n, float f)
{
	float h = n * tan(verticalFov/2) * 2;
	float w = h * aspectRatio;
	buildPerspectiveProjMatLH(mat, w, h, n, f);
}

void buildOrthogonalProjMatLH(pgn::Float4x3* _mat, float w, float h, float n, float f)
{
	pgn::Float4x3& mat = *_mat;

	mat[0][0] = 2 / w;	mat[1][0] = 0;		mat[2][0] = 0;
	mat[0][1] = 0;		mat[1][1] = 2 / h;	mat[2][1] = 0;
	mat[0][2] = 0;		mat[1][2] = 0;		mat[2][2] = 1 / (f - n);
	mat[0][3] = 0;		mat[1][3] = 0;		mat[2][3] = -n / (f - n);
}
