#include <PGN/Graphics/Camera.h>
#include <PGN/Math/Math.h>
#include <PGN/Math/Plane.h>
#include "Viewport.h"

class Camera : public pgn::Camera
{
	enum VFPLANES
	{
		// Far plane of the frustum. That is the plane farest away from the eye
		VF_FAR_PLANE = 0,
		VF_NEAR_PLANE,
		VF_LEFT_PLANE,
		VF_RIGHT_PLANE,
		VF_BOTTOM_PLANE,
		VF_TOP_PLANE,
		VF_PLANE_COUNT
	};

public:
	pgn::Float4x4 projMat;
	pgn::Float4x3 viewMat;
	Viewport viewport;
	pgn::Plane planes[VF_PLANE_COUNT];
	pgn::Float3 camPos;

	Camera();
	virtual void dispose();
	virtual void _free();

	virtual void setFrustumLH(float w, float h, float n, float f);
	virtual void setFrustumFovLH(float verticalFov, float aspectRatio, float n, float f);
	virtual void setOrthoLH(float w, float h, float n, float f);

	virtual void setViewport(int left, int top, int width, int height, int fullHeight);

	virtual void setViewMat(pgn::Float4x3* mat);

	virtual void screenPointToRay(pgn::Float3& begin, pgn::Float3& end, float x, float y);

private:
	void buildVFPlanes();
	// returns the point which is on the far left upper corner inside the the view frustum
	pgn::Float3 getFarLeftUp();
	// returns the point which is on the far left bottom corner inside the the view frustum
	pgn::Float3 getFarLeftDown();
	// returns the point which is on the far right top corner inside the the view frustum
	pgn::Float3 getFarRightUp();
	// returns the point which is on the far right bottom corner inside the the view frustum
	pgn::Float3 getFarRightDown();
	// returns the point which is on the near left upper corner inside the the view frustum
	pgn::Float3 getNearLeftUp();
	// returns the point which is on the near left bottom corner inside the the view frustum
	pgn::Float3 getNearLeftDown();
	// returns the point which is on the near right top corner inside the the view frustum
	pgn::Float3 getNearRightUp();
	// returns the point which is on the near right bottom corner inside the the view frustum
	pgn::Float3 getNearRightDown();
};

inline pgn::Float3 Camera::getFarLeftUp()
{
	pgn::Float3 p;
	planes[Camera::VF_FAR_PLANE].getIntersectionWithPlanes(
		planes[Camera::VF_TOP_PLANE],
		planes[Camera::VF_LEFT_PLANE], p);

	return p;
}

inline pgn::Float3 Camera::getFarLeftDown()
{
	pgn::Float3 p;
	planes[Camera::VF_FAR_PLANE].getIntersectionWithPlanes(
		planes[Camera::VF_BOTTOM_PLANE],
		planes[Camera::VF_LEFT_PLANE], p);

	return p;
}

inline pgn::Float3 Camera::getFarRightUp()
{
	pgn::Float3 p;
	planes[Camera::VF_FAR_PLANE].getIntersectionWithPlanes(
		planes[Camera::VF_TOP_PLANE],
		planes[Camera::VF_RIGHT_PLANE], p);

	return p;
}

inline pgn::Float3 Camera::getFarRightDown()
{
	pgn::Float3 p;
	planes[Camera::VF_FAR_PLANE].getIntersectionWithPlanes(
		planes[Camera::VF_BOTTOM_PLANE],
		planes[Camera::VF_RIGHT_PLANE], p);

	return p;
}

inline pgn::Float3 Camera::getNearLeftUp()
{
	pgn::Float3 p;
	planes[Camera::VF_NEAR_PLANE].getIntersectionWithPlanes(
		planes[Camera::VF_TOP_PLANE],
		planes[Camera::VF_LEFT_PLANE], p);

	return p;
}

inline pgn::Float3 Camera::getNearLeftDown()
{
	pgn::Float3 p;
	planes[Camera::VF_NEAR_PLANE].getIntersectionWithPlanes(
		planes[Camera::VF_BOTTOM_PLANE],
		planes[Camera::VF_LEFT_PLANE], p);

	return p;
}

inline pgn::Float3 Camera::getNearRightUp()
{
	pgn::Float3 p;
	planes[Camera::VF_NEAR_PLANE].getIntersectionWithPlanes(
		planes[Camera::VF_TOP_PLANE],
		planes[Camera::VF_RIGHT_PLANE], p);

	return p;
}

inline pgn::Float3 Camera::getNearRightDown()
{
	pgn::Float3 p;
	planes[Camera::VF_NEAR_PLANE].getIntersectionWithPlanes(
		planes[Camera::VF_BOTTOM_PLANE],
		planes[Camera::VF_RIGHT_PLANE], p);

	return p;
}
