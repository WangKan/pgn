#pragma once
#include "../Common/Interface.h"
namespace pgn {

struct Float3;
struct Float4x3;

class Camera : public Interface
{
public:
	virtual void setFrustumLH(float w, float h, float n, float f) = 0;
	virtual void setFrustumFovLH(float verticalFov, float aspectRatio, float n, float f) = 0;
	virtual void setOrthoLH(float w, float h, float n, float f) = 0;

	virtual void setViewport(int left, int top, int width, int height, int fullHeight) = 0;

	virtual void setViewMat(Float4x3* mat) = 0;

	virtual void screenPointToRay(int x, int y, Float3* origin, Float3* dir) = 0;
};

}
