#include <PGN/Graphics/Camera.h>
#include <PGN/Math/Math.h>
#include "Viewport.h"

class Camera : public pgn::Camera
{
public:
	pgn::Float4x4 projMat;
	pgn::Float4x3 viewMat;
	Viewport viewport;

	Camera();
	virtual void dispose();
	virtual void _free();

	virtual void setFrustumLH(float w, float h, float n, float f);
	virtual void setFrustumFovLH(float fovY, float aspectRatio, float n, float f);
	virtual void setOrthoLH(float w, float h, float n, float f);

	virtual void setViewport(int left, int top, int width, int height, int fullHeight);

	virtual void setViewMat(pgn::Float4x3* mat);
};
