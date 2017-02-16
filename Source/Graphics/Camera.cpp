#include <PGN/Math/Utilities.h>
#include <PGN/Utilities/LinearTransformations.h>
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

	frustum.l = -w / 2.0f;
	frustum.r = w / 2.0f;
	frustum.t = h / 2.0f;
	frustum.b = -h / 2.0f;
	frustum.n = n;
}

void Camera::setFrustumFovLH(float verticalFov, float aspectRatio, float n, float f)
{
	float w, h;
	buildPerspectiveProjMatFovLH(&projMat, verticalFov, aspectRatio, n, f, &w, &h);

	frustum.l = -w / 2.0f;
	frustum.r = w / 2.0f;
	frustum.t = h / 2.0f;
	frustum.b = -h / 2.0f;
	frustum.n = n;
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
}

void Camera::screenPointToRay(int x, int y, pgn::Float3* origin, pgn::Float3* dir)
{
	pgn::Float3 vPos;

	float s = (float)(x - viewport.left) / viewport.width;
	float t = (float)(y - viewport.top) / viewport.height;

	vPos.x = (1.0f - s) * frustum.l + s * frustum.r;
	vPos.y = (1.0f - t) * frustum.t + t * frustum.b;
	vPos.z = frustum.n;

	pgn::Float4x3 invViewMat;
	pgn::calculateInverseViewMat(&viewMat, &invViewMat);

	pgn::transformVertex(&vPos, &invViewMat, origin);

	pgn::Float3 d;
	pgn::transformVector(&vPos, &invViewMat, &d);
	*dir = normalize(d);
}
