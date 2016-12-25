#pragma once
#include "../Common/Interface.h"
namespace pgn {

class PointLight : public Interface
{
public:
	virtual void setIntensity(float r, float g, float b, float spec) = 0;
	virtual void setRadius(float r) = 0;
};

}
