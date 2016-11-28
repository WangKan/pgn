#include "Camera.h"
#include "Graphics.h"
#include "Matrix.h"

Camera::Camera()
{
}

void Camera::dispose()
{
}

pgn::Camera* Graphics::createCamera()
{
	return debug_new Camera;
}

void Camera::_free()
{
	delete this;
}

void Camera::setFrustumLH(float w, float h, float n, float f)
{
	buildPerspectiveProjMatLH(&projMat, w, h, n, f);
}

void Camera::setFrustumFovLH(float fovY, float aspectRatio, float n, float f)
{
	buildPerspectiveProjMatFovLH(&projMat, fovY, aspectRatio, n, f);
}

void Camera::setOrthoLH(float w, float h, float n, float f)
{
	buildOrthogonalProjMatLH(&projMat.float4x3, w, h, n, f);
	projMat[3][0] = 0;
	projMat[3][1] = 0;
	projMat[3][2] = 0;
	projMat[3][3] = 1;
}

void Camera::setViewport(int left, int top, int width, int height, int fullHeight)
{
	viewport.left = left;
	viewport.top = top;
	viewport.width = width;
	viewport.height = height;
	viewport.fullHeight = fullHeight;
}

void Camera::setViewMat(pgn::Float4x3* mat)
{
	viewMat = *mat;

	buildVFPlanes();
}

void Camera::buildVFPlanes()
{
	pgn::Float4x4 modView;
	modView = viewMat;
	modView[3][0] = 0;
	modView[3][1] = 0;
	modView[3][2] = 0;
	modView[3][3] = 1;

	camPos[0] = -modView[0][3];
	camPos[1] = -modView[1][3];
	camPos[2] = -modView[2][3];

	pgn::Float4x4 mat;
	pgn::mul(&modView, &projMat, &mat);

	planes[VF_LEFT_PLANE].normal[0] = mat[3][0] + mat[0][0];
	planes[VF_LEFT_PLANE].normal[1] = mat[3][1] + mat[0][1];
	planes[VF_LEFT_PLANE].normal[2] = mat[3][2] + mat[0][2];
	planes[VF_LEFT_PLANE].distance = mat[3][3] + mat[0][3];

	// right clipping plane
	planes[VF_RIGHT_PLANE].normal[0] = mat[3][0] - mat[0][0];
	planes[VF_RIGHT_PLANE].normal[1] = mat[3][1] - mat[0][1];
	planes[VF_RIGHT_PLANE].normal[2] = mat[3][2] - mat[0][2];
	planes[VF_RIGHT_PLANE].distance = mat[3][3] - mat[0][3];

	// top clipping plane
	planes[VF_TOP_PLANE].normal[0] = mat[3][0] - mat[1][0];
	planes[VF_TOP_PLANE].normal[1] = mat[3][1] - mat[1][1];
	planes[VF_TOP_PLANE].normal[2] = mat[3][2] - mat[1][2];
	planes[VF_TOP_PLANE].distance = mat[3][3] - mat[1][3];

	// bottom clipping plane
	planes[VF_BOTTOM_PLANE].normal[0] = mat[3][0] + mat[1][0];
	planes[VF_BOTTOM_PLANE].normal[1] = mat[3][1] + mat[1][1];
	planes[VF_BOTTOM_PLANE].normal[2] = mat[3][2] + mat[1][2];
	planes[VF_BOTTOM_PLANE].distance = mat[3][3] + mat[1][3];

	// far clipping plane
	planes[VF_FAR_PLANE].normal[0] = mat[3][0] - mat[2][0];
	planes[VF_FAR_PLANE].normal[1] = mat[3][1] - mat[2][1];
	planes[VF_FAR_PLANE].normal[2] = mat[3][2] - mat[2][2];
	planes[VF_FAR_PLANE].distance = mat[3][3] - mat[2][3];

	// near clipping plane
	planes[VF_NEAR_PLANE].normal[0] = mat[2][0];
	planes[VF_NEAR_PLANE].normal[1] = mat[2][1];
	planes[VF_NEAR_PLANE].normal[2] = mat[2][2];
	planes[VF_NEAR_PLANE].distance = mat[2][3];

	// normalize normals
	for (int i = 0; i != VF_PLANE_COUNT; ++i)
	{
		pgn::Float3& n = planes[i].normal;
		float len = -1 / sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
		n[0] *= len;
		n[1] *= len;
		n[2] *= len;
		planes[i].distance *= len;
	}
}

void Camera::screenPointToRay(pgn::Float3& begin, pgn::Float3& end, float x, float y)
{
	pgn::Float3 farLeftUp = getFarLeftUp();
	pgn::Float3 farRightUp = getFarRightUp();
	pgn::Float3 farLeftDown = getFarLeftDown();
	pgn::Float3 leftToRight;
	pgn::Float3 upToDown;
	for(int i = 0; i < 3; ++i)
		leftToRight[i] = farRightUp[i] - farLeftUp[i];
	for (int i = 0; i < 3; ++i)
		upToDown[i] = farLeftDown[i] - farLeftUp[i];

	float dx = (x - viewport.left) / viewport.width;
	float dy = (y - viewport.top) / viewport.height;

	for (int i = 0; i < 3; ++i)
		begin[i] = camPos[i];

	for (int i = 0; i < 3; ++i)
		end[i] = farLeftUp[i] + (leftToRight[i] * dx) + (upToDown[i] * dy);
}

